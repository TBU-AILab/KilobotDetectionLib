#include "yolo_detector.h"

#include <random>

namespace Kilobot_detection_library {


    /**
     * @brief Default constructor.
     * @param fileName Name of the file representing net structure
     * @param is_cuda True if CUDA execution need to be run.
     */
    YoloDetector::YoloDetector(const std::string &fileName, bool is_cuda) {
        if (!fileName.empty())
            LoadNet(fileName, is_cuda);
    }


    /**
     * @brief Loads model from path and determinates running on CPU or GPU.
     *
     * @param pathToFile Relative path to file with pretrained YOLOV5 model.
     * @param is_cuda If true, function tries to run Net with CUDA backend.
     *
     * @result Result information @sa YD_RESULT
    */
    YD_RESULT YoloDetector::LoadNet(const std::string &pathToFile, bool is_cuda) {
        if (pathToFile.empty()) {
            return YD_RESULT::YD_ARGS_ERROR;
        }
        // loads a neural network from provided file
        _net = cv::dnn::readNet(pathToFile);

        if (_net.empty()) {
            return YD_RESULT::YD_ERROR;
        }

        if (is_cuda) {
#ifdef ENABLE_DEBUG
            std::cout << "Running with CUDA\n";
#endif
            _net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            _net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
        } else {
#ifdef ENABLE_DEBUG
            std::cout << "Running on CPU\n";
#endif
            _net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            _net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }

        return YD_RESULT::YD_OK;
    }


    /**
     * @brief Method used for Kilobot detection in frame.
     *
     * @param frame Frame used for detection.
     * @param output Vector in which detections will be stored.
     * @param scoreVal Minimum value for kilobot score. Values greater than this are considered to be Kilobots.
     * @param confVal Minimum value for confidence used by Net. Values greater than this are considered to be objects.
     * @param nmsVal Value usedfor non-maximum suppression.
     *
     * @result Result information @sa YD_RESULT
    */
    YD_RESULT
    YoloDetector::Detect(const cv::Mat &frame, std::vector<Kilobot> &output, float score, float conf, float nms) {
        if (_net.empty()) {
#ifdef ENABLE_DEBUG
            std::cout << "There is no Net available for detection!\n";
#endif
            return YD_RESULT::YD_ERROR;
        }
        // create BLOB, set it as Net input and try to find Kilobots
        cv::Mat blob;
        float x_factor; // final scale factor x-axis
        float y_factor; // final scale factor y-axis
        _createInputBlob(frame, blob, x_factor, y_factor);

        // process the blob by the NN
        std::vector<std::vector<Mat>> nnOutput;
        _net.setInput(blob);
        _net.forward(nnOutput, {"output"});

        // parse DNN results
        const int rows = 25200; // number of outputs for Net trained on 640x640 px
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;
        _parseNNResults(nnOutput, confidences, boxes, rows, x_factor, y_factor, score, conf);

        // apply Non maximum suppression and reduce number of multiply detected objets 
        std::vector<int> nms_result;
        cv::dnn::NMSBoxes(boxes, confidences, score, nms, nms_result);

        // convert results to a collection of Kilobots
        for (auto idx: nms_result) {
            Kilobot result;
            result.confidence = confidences[idx];
            result.box = boxes[idx];
            result.id = -1;
            result.undetected = 0;

            result.color = Scalar(0, 0, 0);
            result.LEDcolor = Scalar(0, 0, 0);

            output.push_back(result);
        }
        return YD_RESULT::YD_OK;
    }

    /**
     * @brief Convert input frame into a squared frame (missing pixels are set to 0 values)
     * @param source Original frame
     * @return Squared version of the frame
     */
    cv::Mat YoloDetector::_format(const cv::Mat &source) {
        int col = source.cols;
        int row = source.rows;
        int _max = MAX(col, row);
        cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);

        source.copyTo(result(cv::Rect(0, 0, col, row)));
        return result;
    }

    /**
     * @brief Prepare input blob for a NN
     *
     * This function takes input frame  and perform all necessary operation in order to create an input blob for NN. At first, the image is extended to be square image.
     * Next, the image is converted to a DNN blob (scaled to a correct size and input colors are swapped from BGR to RGB). Finally, the scale factors are computed in order to reconstruct
     * original position and scale of detected objects.
     * @param inFrame Input frame in form of BGR
     * @param outBlob Output DNN blob
     * @param outXFactor X scale factor
     * @param outYFactor Y scale factor
     * @return result information @sa YD_RESULT
     */
    YD_RESULT
    YoloDetector::_createInputBlob(const cv::Mat &inFrame, cv::Mat &outBlob, float &outXFactor, float &outYFactor) {
        //change the format of and NN
        auto input_image = _format(inFrame);

        // create BLOB, set it as Net input and try to find Kilobots
        cv::dnn::blobFromImage(input_image, outBlob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true,
                               false);
        if (outBlob.empty()) return YD_RESULT::YD_ERROR;

        outXFactor = (float) input_image.cols / INPUT_WIDTH;
        outYFactor = (float) input_image.rows / INPUT_HEIGHT;

        return YD_RESULT::YD_OK;
    }

    /**
     * @brief Parse an output form DNN to a set of boxes and confidences.
     *
     * @param nnOutput the set of vectors representing output from DNN network
     * @param confidences output vector of confidence numbers
     * @param boxes output vector of detected objects boundary boxes
     * @param resultRows number of suspected row in the DNN output
     * @param xScale x scale factor @sa _createInputBlob
     * @param yScale y scale factor @sa _createInputBlob
     * @param scoreVal required score value (only higher values will be accepted)
     * @param confVal required confidence value (only higher or equal values will be accepted)
     * @return result information @sa YD_RESULT
     */
    YD_RESULT
    YoloDetector::_parseNNResults(const std::vector<std::vector<Mat>> &nnOutput, std::vector<float> &confidences,
                                  std::vector<cv::Rect> &boxes, int resultRows, float xScale,
                                  float yScale, float scoreVal, float confVal) {
        //iterate over the data
        for (const auto &layer: nnOutput) {
            for (const auto &result: layer) {
                for (int i = 0; i < resultRows; ++i) {

                    // [0, 1,   2,      3,        4,       5]
                    // [x, y, width, height, confidence, score]
                    struct tPredict {
                        float x;
                        float y;
                        float width;
                        float height;
                        float confidence;
                        float score;
                    };
                    static_assert(sizeof(tPredict) == (sizeof(float) * 6));

                    tPredict prediction = result.at<tPredict>(i);
                    if ((prediction.confidence >= confVal) &&
                        (prediction.score > scoreVal)) { // data[5] == kilobot score
                        confidences.push_back(prediction.confidence);
                        auto left = int((prediction.x - 0.5 * prediction.width) * xScale);
                        auto top = int((prediction.y - 0.5 * prediction.height) * yScale);
                        auto width = int(prediction.width * xScale);
                        auto height = int(prediction.height * yScale);
                        boxes.emplace_back(left, top, width, height);
                    }
                }
            }
        }

        return YD_RESULT::YD_ERROR;
    }
}