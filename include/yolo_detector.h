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
        /**
         * @brief Constructor
        */
        YoloDetector();

        /**
         * @brief Loads model from path and determinates running on CPU or GPU.
         *
         * @param pathToFile Relative path to file with pretrained YOLOV5 model.
         * @param is_cuda If true, function tries to run Net with CUDA backend.
         *
         * @result Result information @sa YD_RESULT
        */
        YD_RESULT LoadNet(const string &pathToFile, bool is_cuda);

        /**
         * @brief Method used for Kilobot detection in frame.
         *
         * @param frame Frame used for detection.
         * @param output Vector in which detections will be stored.
         * @param scoreVal Minimum value for kilobot score. Values greater than this are considered to be Kilobots.
         * @param confVal Minimum value for confidence used by Net. Values greater than this are considered to be objects.
         * @param nmsVal Value usedfor non-maximum suppression.
         *
         * @result Result information @sa YD_RESULT
        */
        YD_RESULT Detect(cv::Mat& frame, std::vector<Kilobot>& output, float scoreVal, float confVal, float nmsVal);


    protected:
        YD_RESULT _createInputBlob(cv::Mat inFrame, cv::Mat &outBlob, float& outXFactor, float &outYFactor );
        YD_RESULT _parseNNResults(const std::vector<std::vector<Mat>> &nnOutput, std::vector<float> &confidences,
                                  std::vector<cv::Rect> &boxes, int resultRows, float xScale,
                                  float yScale, float scoreVal, float confVal);
    private:

        /**
         * @brief Expand the source image to a square image, where the bigger dimension is used.
         * @param source Source image
         * @return Extended image
         */
        cv::Mat _format(const cv::Mat& source);

    private:
        cv::dnn::Net _net; /*!< Loaded DNN Net model. */
        std::vector<cv::Mat> _outputs; /*!< Output of DNN detection. */
    };
}
