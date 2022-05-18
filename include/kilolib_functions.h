#pragma once

/*****************************************************************//**
 * @file   kilolib_functions.h
 * @brief  Helpful functions for kilolib library.
 *
 * @author Petr Svoboda
 * @date   May 2022
***********************************************************************/

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

namespace kilolib {
    static const double F = 1240;// focal length
    static const double kilobotSize = 3.3; // real Kilobot size in centimeters

    /**
     * @brief Function that finds LED inside Kilobot bounding box.
     *
     * @param frame Current frame.
     * @param box Bounding nox of detected Kilobot.
     * @param thresh Threshold value used for detection.
     * @param minArea Minimum area - percentage value in decimal of detected box.
     *
     * @return Center point of detected LED. Returns Point(-1,-1) if LED wasn't detected.
    */
    Point findLED(Mat frame, Rect box, double thresh, double minArea);

    /**
     * @brief Measures distance to detected Kilobot.
     *
     * @param box Bounding box of detected Kilobot.
     * @param frame Current frame.
     * @param display True if distance should be displayed in current frame.
     * @param textColor Text color.
     * @return
    */
    double measureDistance(Rect box, Mat frame, bool display = true, Scalar textColor = Scalar(255, 0, 255));

    /**
     * @brief Detects single Kilobot in image center
     *
     * @param frame Current frame.
     * @param imgPath Path to image file.
     * @param percentage The percentage portion of image around the center in which Kilobot will be searched.
     * @param thresh Threshold value used for detection by findContours.
     * @param measureDist True if distance should be displayed in current frame.
     * @return
    */
    void centerKilobotDetection(Mat& frame, string imgPath, int percentage, int thresh, bool measureDist = true);
}
