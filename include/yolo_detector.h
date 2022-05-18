#pragma once

/*****************************************************************//**
 * @file   yolo_detector.h
 * @brief  Class used for Kilobot detection. It uses YOLOV5 model from https://github.com/ultralytics/yolov5.
 *         You should be able to use different models, but this feature isn't tested.
 *
 * @author Petr Svoboda
 * @date   April 2022
***********************************************************************/

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
        /**
         * @brief Constructor
        */
        YoloDetector();

        /**
         * @brief Loads model from path and determinates running on CPU or GPU.
         *
         * @param pathToFile Relative path to file with pretrained YOLOV5 model.
         * @param is_cuda If true, function tries to run Net with CUDA backend.
        */
        void LoadNet(string pathToFile, bool is_cuda);

        /**
         * @brief Method used for Kilobot detection in frame.
         *
         * @param frame Frame used for detection.
         * @param output Vector in which detections will be stored.
         * @param scoreVal Minimum value for kilobot score. Values greater than this are considered to be Kilobots.
         * @param confVal Minimum value for confidence used by Net. Values greater than this are considered to be objects.
         * @param nmsVal Value usedfor non-maximum suppression.
        */
        void Detect(cv::Mat& frame, std::vector<Kilobot>& output, float scoreVal, float confVal, float nmsVal);

    private:
        cv::Mat _format(const cv::Mat& source);

    private:
        cv::dnn::Net _net; /*!< Loaded DNN Net model. */
        std::vector<cv::Mat> _outputs; /*!< Output of DNN detection. */
    };
}
