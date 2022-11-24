#pragma once

/*****************************************************************//**
 * @file   kilobot.h
 * @brief  Struct Kilobot
 *
 * @author Petr Svoboda
 * @date   April 2022
***********************************************************************/

#include "opencv2/opencv.hpp"

using namespace cv;

namespace Kilobot_detection_library {

    struct Kilobot
    {
        float confidence; /*!< confidence of Kilobot detection */
        cv::Rect box; /*!< rect containing bounding box */
        int id; /*!< Kilobot id */
        int undetected; /*!< number of frames where Kilobot wasn't detected */
        std::vector<Point> led; /*!< vector with detected LED centers */
        cv::Point led_pos;
        int led_col;
        std::vector<Point> trajectory; /*!< vector containing trajectory */
        cv::Scalar color; /*!< Kilobot color */
        cv::Scalar LEDcolor; /*!< LED color */
        Point center() { return Point(box.x + box.width / 2, box.y + box.height / 2); }
    };
}