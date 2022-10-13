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
#include <string>
#include <vector>

#include "kilobot.h"

using namespace std;
using namespace cv;

static const float INPUT_WIDTH = 640.0;
static const float INPUT_HEIGHT = 640.0;

namespace kilolib {

    /**
     * @brief Result data type for functions from YoloDetector class
     */
    enum class YD_RESULT {
        YD_OK,          /**< correct function execution */
        YD_ARGS_ERROR,  /**< unexpected or incorrect input parameters */
        YD_ERROR        /**< unspecified error */
    };

    class YoloDetector {
    public:

        explicit YoloDetector(const std::string &fileName = "", bool is_cuda = false);

        YD_RESULT LoadNet(const string &pathToFile, bool is_cuda);

        YD_RESULT Detect(cv::Mat &frame, std::vector<Kilobot> &output, float scoreVal, float confVal, float nmsVal);

        /**
         * @brief Check if some network is loaded or not.
         * @return True if the network is correctly loaded, false otherwise.
         */
        bool isLoaded() const { return !_net.empty(); }

    protected:
        YD_RESULT _createInputBlob(const cv::Mat &inFrame, cv::Mat &outBlob, float &outXFactor, float &outYFactor);

        YD_RESULT _parseNNResults(const std::vector<std::vector<Mat>> &nnOutput, std::vector<float> &confidences,
                                  std::vector<cv::Rect> &boxes, int resultRows, float xScale,
                                  float yScale, float scoreVal, float confVal);

        cv::Mat _format(const cv::Mat &source);

        cv::dnn::Net _net; /*!< Loaded DNN Net model. */
    };
}
