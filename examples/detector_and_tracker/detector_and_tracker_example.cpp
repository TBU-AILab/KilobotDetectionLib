/*****************************************************************//**
 * @file   detector_and_tracker_example.cpp
 * @brief  Example that detects Kilobots in video file and then tracks them.
 *         Displays IDs of detected Kilobots.
 *
 * @author Petr Svoboda
 * @date   May 2022
***********************************************************************/

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "kilobot.h"
#include "euclidian_tracker.h"
#include "yolo_detector.h"
#include "kilolib_functions.h"

using namespace kilolib;
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Creates YOLO detector and loads DNN model "best.onnx"
    YoloDetector detector;
    detector.LoadNet("best.onnx", true);

    // Open video "bodyguard.mp4"
    cv::VideoCapture capture("bodyguard.mp4");

    if (!capture.isOpened())
    {
        std::cerr << "Error opening video file\n";
        return -1;
    }

    // default values 
    int nms = 0;
    int score = 5;
    int conf = 5;
    int t = 200;
    int per = 50;

    //create a window called "Controls" with trackbars
    namedWindow("Controls", WINDOW_AUTOSIZE); 
    createTrackbar("NMS", "Controls", &nms, 10);
    createTrackbar("Score", "Controls", &score, 10);
    createTrackbar("Confidence", "Controls", &conf, 10);
    createTrackbar("Threshold", "Controls", &t, 255);
    createTrackbar("Percentage", "Controls", &per, 50);

    // vectors, where detections on current and previous frame will be stored
    std::vector<Kilobot> output;
    std::vector<Kilobot> prevOutput;

    cv::Mat frame;

    EuclidianTracker tracker;

    while (true)
    {
        // read frame
        capture.read(frame);

        if (frame.empty())
        {
            std::cout << "End of stream\n";
            break;
        }

        // detect Kilobots
        detector.Detect(frame, output, (float)(nms + 1) / 10, (float)(conf + 1) / 10, (float)(score + 1) / 10);

        // track Kilobots
        tracker.Track(frame, output, prevOutput, (double)t + 1, per / 10);

        // show output
        imshow("output", frame);

        if (cv::waitKey(1) != -1)
        {
            capture.release();
            std::cout << "finished by user\n";
            break;
        }

        // set current detections to prevOutput for calculations on next frame
        prevOutput = output;
        // clear current detections
        output.clear();
    }

    cv::waitKey(0);

    return 0;
}