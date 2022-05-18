#pragma once

/*****************************************************************//**
 * @file   euclidian_tracker.h
 * @brief  Class Euclidiant Tracker. It's designed to track previously detected Kilobots.
 *
 * @author Petr Svoboda
 * @date   April 2022
***********************************************************************/

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
        /**
         * @brief A Constructor
         *
         * @param lastId Number (ID) of the first tracked Kilobot. Will be incremented for other tracked Kilobots.
         * @param maxDist Maximum distance that Kilobot can move between two frames.
         * @param maxUndetected Maximum number of frames, where Kilobot wasn't detected.
        */
        EuclidianTracker(int lastId = defId, int maxDist = defDist, int maxUndetected = defUndetected);

        /**
         * @brief Resets all parameters to new values and CLEARS _prevUndetected vector.
         * After this function is called tracking starts from the beginning .
         *
         * @param lastId Number (ID) of the first tracked Kilobot. Will be incremented for other tracked Kilobots.
         * @param maxDist Maximum distance that Kilobot can move between two frames.
         * @param maxUndetected Maximum number of frames, where Kilobot wasn't detected.
        */
        void Reset(int lastId = defId, int maxDist = defDist, int maxUndetected = defUndetected);

        /**
         * @brief Function that tracks Kilobots in current and previous frame.
         * It assigns IDs to newly detected Kilobots and tracks them with detections from previous frame.
         *
         * @param frame Current frame.
         * @param output Current detected Kilobots.
         * @param prevOutput Kilobots detected in the previous frame.
         * @param LEDthreshVal Threshold value for LED detection.
         * @param LEDPercentageVal Percentage value of minimum area that detected LED has to occupy.
        */
        void Track(Mat frame, std::vector<Kilobot>& output, std::vector<Kilobot>& prevOutput,
            double LEDthreshVal, double LEDPercentageVal);

    private:
        /**
         * @brief Private function which looks for specific Kilobot in vector containing detections.
         *
         * @param frame Current frame.
         * @param obj Specific Kilobot.
         * @param objects Vector of Kilobots to look.
         * @param LEDthreshVal Threshold value for LED detection.
         * @param LEDPercentageVal Percentage value of minimum area that detected LED has to occupy.
        */
        void _findObject(Mat& frame, Kilobot& obj, std::vector<Kilobot>& objects, double LEDthreshVal, double LEDPercentageVal);

    private:

        int _maxDist; /*!< Maximum distance that Kilobot can move between two frames. */
        int _maxUndetected;/*!< Maximum number of frames on which Kilobot wasn't detected. */
        int _lastId; /*!< Number (ID) of the first tracked Kilobot. Will be incremented for other tracked Kilobots. */

        std::vector<Kilobot> _prevUndetected; /*!< Kilobots detected in the previous frame. */
    };
}