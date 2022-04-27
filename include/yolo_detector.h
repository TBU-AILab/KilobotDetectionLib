#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include "kilobot.h"

using namespace std;
using namespace cv;

static const float INPUT_WIDTH = 640.0;
static const float INPUT_HEIGHT = 640.0;

namespace kilolib {

    class YoloDetector {
    public:
        YoloDetector();

        void LoadNet(string pathToFile, bool is_cuda);

        void Detect(cv::Mat& image, std::vector<Kilobot>& output, float scoreVal, float confVal, float nmsVal);

    private:
        cv::Mat _format(const cv::Mat& source);

    private:
        cv::dnn::Net _net;
        std::vector<cv::Mat> _outputs;
    };
}
