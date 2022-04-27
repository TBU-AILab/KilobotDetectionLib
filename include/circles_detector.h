#pragma once

#include <fstream>
#include <iostream>
#include <random>

#include "opencv2/opencv.hpp"

#include "kilobot.h"

using namespace std;
using namespace cv;

namespace kilolib {

    class CirclesDetector {
    public:
        CirclesDetector() {};

        void Detect(cv::Mat& image, std::vector<Kilobot>& output, float scoreVal,
            float confVal, float nmsVal, double LEDthreshVal, double LEDPercentageVal) {};

    private:
        std::vector<cv::Mat> _outputs;
    };
}
