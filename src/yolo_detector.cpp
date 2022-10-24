#include "yolo_detector.h"

#include <random>

namespace kilolib {

    YoloDetector::YoloDetector()
    {
    }

    void YoloDetector::LoadNet(string pathToFile, bool is_cuda)
    {
        // naète model neuronové sítì
        _net = cv::dnn::readNet(pathToFile);

        if (is_cuda)
        {
            std::cout << "Running with CUDA\n";
            _net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            _net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
        }
        else
        {
            std::cout << "Running on CPU\n";
            _net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            _net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }
    }

    void YoloDetector::Detect(cv::Mat& frame, std::vector<Kilobot>& output, float score, float conf, float nms)
    {
        if (_net.empty()) {
            std::cout << "There is no Net available for detection!\n";
            return;
        }

        cv::Mat blob;

        auto input_image = _format(frame);

        // create BLOB, set it as Net input and try to find Kilobots
        cv::dnn::blobFromImage(input_image, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);
        _net.setInput(blob);

        _net.forward(_outputs, _net.getUnconnectedOutLayersNames());

        float x_factor = input_image.cols / INPUT_WIDTH;
        float y_factor = input_image.rows / INPUT_HEIGHT;

        // get output from Net
        float* data = (float*)_outputs[0].data;

        // [0, 1,   2,      3,        4,       5]
        // [x, y, width, height, confidence, score]
        const int dimensions = 6;

        // number of outputs for Net trained on 640x640 px
        const int rows = 25200;

        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        // filter outputs and get only results where Net was confident
        for (int r = 0; r < rows; ++r) {
            if (data[4] >= conf) { // data[4] == confidence
                if (data[5] > score) { // data[5] == kilobot score

                    confidences.push_back(data[4]);

                    float x = data[0];
                    float y = data[1];
                    float w = data[2];
                    float h = data[3];
                    int left = int((x - 0.5 * w) * x_factor);
                    int top = int((y - 0.5 * h) * y_factor);
                    int width = int(w * x_factor);
                    int height = int(h * y_factor);
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }

            // move to next output
            data += dimensions;
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