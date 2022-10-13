#include "yolo_detector.h"

#include <random>

namespace kilolib {

    YoloDetector::YoloDetector()
    {
    }

    YD_RESULT YoloDetector::LoadNet(string pathToFile, bool is_cuda)
    {
        if (pathToFile.empty()){
            return YD_RESULT::YD_ARGS_ERROR;
        }

        // loads a neural network from provided file
        _net = cv::dnn::readNet(pathToFile);

        if (_net.empty()){
            return YD_RESULT::YD_ERROR;
        }

        if (is_cuda)
        {
#ifdef ENABLE_DEBUG
            std::cout << "Running with CUDA\n";
#endif
            _net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            _net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
        }
        else
        {
#ifdef ENABLE_DEBUG
            std::cout << "Running on CPU\n";
#endif
            _net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            _net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }

        return YD_RESULT::YD_OK;
    }

    YD_RESULT YoloDetector::Detect(cv::Mat& frame, std::vector<Kilobot>& output, float score, float conf, float nms)
    {
        if (_net.empty()) {
#ifdef ENABLE_DEBUG
            std::cout << "There is no Net available for detection!\n";
#endif
            return YD_RESULT::YD_ERROR;
        }



        //change the format of and NN
        auto input_image = _format(frame);

        // create BLOB, set it as Net input and try to find Kilobots
        cv::Mat blob;
        cv::dnn::blobFromImage(input_image, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true,
                               false);
        if (blob.empty()) return YD_RESULT::YD_ERROR;
        // set  the blob as the NN input
        _net.setInput(blob);
        // process the blob by the NN
        std::vector<std::vector<Mat>> nnOutput;
        _net.forward(nnOutput, {"output"});

        float x_factor = input_image.cols / INPUT_WIDTH;
        float y_factor = input_image.rows / INPUT_HEIGHT;

        // number of outputs for Net trained on 640x640 px
        const int rows = 25200;

        // prepare output vectors
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        //iterate over the data
        for (const auto &layer: nnOutput) {
            for (const auto &result: layer) {
                for (int i = 0; i < rows; ++i) {

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
                    if (prediction.confidence >= conf) { // data[4] == confidence
                        if (prediction.score > score) { // data[5] == kilobot score
                            confidences.push_back(prediction.confidence);

                            float x = prediction.x;
                            float y = prediction.y;
                            float w = prediction.width;
                            float h = prediction.height;
                            int left = int((x - 0.5 * w) * x_factor);
                            int top = int((y - 0.5 * h) * y_factor);
                            int width = int(w * x_factor);
                            int height = int(h * y_factor);
                            boxes.push_back(cv::Rect(left, top, width, height));


                        }
                    }
                }
            }
        }

        // apply Non maximum suppression and reduce number of multiply detected objets 
        std::vector<int> nms_result;
        cv::dnn::NMSBoxes(boxes, confidences, score, nms, nms_result);

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, 255); // define the range

        for (int n = 0; n < nms_result.size(); n++) {
            int idx = nms_result[n];
            Kilobot result;
            result.confidence = confidences[idx];
            result.box = boxes[idx];
            result.id = -1;
            result.undetected = 0;

            result.color = Scalar(distr(gen), distr(gen), distr(gen));
            result.LEDcolor = Scalar(distr(gen), distr(gen), distr(gen));

            output.push_back(result);
        }
        return YD_RESULT::YD_OK;
    }

    cv::Mat YoloDetector::_format(const cv::Mat& source)
    {
        int col = source.cols;
        int row = source.rows;
        int _max = MAX(col, row);
        cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);

        source.copyTo(result(cv::Rect(0, 0, col, row)));
        return result;
    }
}