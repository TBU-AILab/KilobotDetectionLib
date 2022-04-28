EXAMPLE USAGE OF YoloDetector AND EuclidianTracker CLASSES

#include <opencv2/opencv.hpp>
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
    YoloDetector detector;
    detector.LoadNet("res/bestCUDA.onnx", true);

    cv::VideoCapture capture("res/kamera1.mp4");
    capture.set(cv::CAP_PROP_FRAME_WIDTH, INPUT_WIDTH);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, INPUT_HEIGHT);
    if (!capture.isOpened())
    {
        std::cerr << "Error opening video file\n";
        return -1;
    }

    int nms = 0;
    int score = 5;
    int conf = 5;
    int t = 200;
    int per = 50;

    namedWindow("Controls", WINDOW_AUTOSIZE); //create a window called "Controls"
    createTrackbar("NMS", "Controls", &nms, 10);
    createTrackbar("Score", "Controls", &score, 10);
    createTrackbar("Confidence", "Controls", &conf, 10);
    createTrackbar("Threshold", "Controls", &t, 255);
    createTrackbar("Percentage", "Controls", &per, 50);

    std::vector<Kilobot> output;
    std::vector<Kilobot> prevOutput;

    cv::Mat frame;

    EuclidianTracker tracker;

    while (true)
    {
        capture.read(frame);

        if (frame.empty())
        {
            std::cout << "End of stream\n";
            break;
        }

        detector.Detect(frame, output, (float)(nms + 1) / 10, (float)(conf + 1) / 10, (float)(score + 1) / 10);

        tracker.Track(frame, output, prevOutput, (double)t + 1, per / 10);

        imshow("output", frame);

        if (cv::waitKey(1) != -1)
        {
            capture.release();
            std::cout << "finished by user\n";
            break;
        }

        prevOutput = output;
        output.clear();
    }

    cv::waitKey(0);

    return 0;
}