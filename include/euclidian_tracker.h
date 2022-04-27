#pragma once

#include "opencv2/opencv.hpp"

#include "kilobot.h"

using namespace std;
using namespace cv;

static const int defId = 1;
static const int defDist = 30;
static const int defUndetected = 100;

namespace kilolib {

    class EuclidianTracker {
    public:
        EuclidianTracker(int lastId = defId, int maxDist = defDist, int maxUndetected = defUndetected);

        void Reset(int lastId = defId, int maxDist = defDist, int maxUndetected = defUndetected);
        void Track(Mat frame, std::vector<Kilobot>& output, std::vector<Kilobot>& prevOutput, double LEDthreshVal, double LEDPercentageVal);

    private: 
        void _findObject(Mat& frame, Kilobot& obj, std::vector<Kilobot>& objects, double LEDthreshVal, double LEDPercentageVal);

    private: 
        int _maxDist;
        int _maxUndetected;

        int _lastId;

        std::vector<Kilobot> _prevUndetected;
    };
}