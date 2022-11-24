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
#include "simpletracker.h"
#include <array>
#include <unordered_map>

using namespace Kilobot_detection_library;
using namespace cv;
using namespace std;

/**
 * @brief Function to detect LED of each kilobot. Uses the separated image of kilobot and ligh channel to separate only the enlightened LED.
 * @param frame original frame from input video source
 * @param kilobot pointer to the kilobot structure
 * @return
 */

bool detect_LED(cv::Mat extracted_kilobot, std::shared_ptr<Kilobot_detection_library::Kilobot> kilobot){

    std::vector<KeyPoint> blob_keypoints;
    cv::Mat cropped_kilobot;
    cv::Mat mask = cv::Mat::zeros(extracted_kilobot.rows,extracted_kilobot.cols, CV_8U);
    cv::cvtColor(extracted_kilobot, extracted_kilobot, COLOR_BGR2GRAY);
    cv::circle(mask, cv::Point(extracted_kilobot.rows / 2, extracted_kilobot.cols / 2), (extracted_kilobot.cols / 2), Scalar(255, 0, 0), -1, 8, 0);
    extracted_kilobot.copyTo(cropped_kilobot, mask);
    extracted_kilobot = cropped_kilobot;

    cv::cvtColor(extracted_kilobot, extracted_kilobot, COLOR_GRAY2BGR);
    cv::cvtColor(extracted_kilobot, extracted_kilobot, COLOR_BGR2HLS);
    cv::extractChannel(extracted_kilobot, extracted_kilobot, 1);
    cv::threshold(extracted_kilobot, extracted_kilobot, 230, 255, cv::THRESH_BINARY);

    SimpleBlobDetector::Params blob_det_parameters;
    blob_det_parameters.filterByColor = 1;
    blob_det_parameters.blobColor = 255;
    blob_det_parameters.filterByArea = 0;
    blob_det_parameters.filterByCircularity = 0;
    blob_det_parameters.filterByConvexity = 0;
    blob_det_parameters.filterByInertia = 0;
    blob_det_parameters.minThreshold = 230;
    blob_det_parameters.maxThreshold = 255;

    cv::Ptr<cv::SimpleBlobDetector> LED_detector = cv::SimpleBlobDetector::create(blob_det_parameters);
    LED_detector->detect(extracted_kilobot, blob_keypoints);

    if (!blob_keypoints.empty()) {
        float dist = sqrt(pow((blob_keypoints[0].pt.x - kilobot->box.width/2), 2) + pow((blob_keypoints[0].pt.y - kilobot->box.height/2), 2));
        if (dist > 0.6*kilobot->box.width/2){
            kilobot->led_pos = cv::Point(blob_keypoints[0].pt.x, blob_keypoints[0].pt.y);
            return true;
        }
        if (blob_keypoints.empty()){
            /*cout << kilobot->id
                 << " LED not found \n"
                 << "distance: "
                 << dist
                 << "\n";*/
            return false;
        }
        /*cout << kilobot->id
             << " distance too small \n"
             << "distance: "
             << dist
             << "\n";*/
    }

    return false;


}
void LED_color(cv::Mat extracted_kilobot, std::shared_ptr<Kilobot_detection_library::Kilobot> kilobot){

//    cv::cvtColor(extracted_kilobot,extracted_kilobot,COLOR_BGR2HSV);
    // kilobot LED size = kilobot box size / 10
    // kilobot led_pos = established
    // kilobot led_box = x kilobot_led.x = kilobot box size / 2

    enum colors
    {
        BLACK,
        WHITE,
        RED,
        GREEN,
        BLUE,
        YELLOW,
        PURPLE,
        ORANGE,
        GRAY,
    };

    struct tColor{
        cv::Scalar low;
        cv::Scalar high;
    };
    std::array<int, 9> colorValues = {0};

    std::unordered_map<colors, tColor> color_ranges;
    tColor tempColor;

    tempColor.low=cv::Scalar(0, 0, 0);
    tempColor.high=cv::Scalar(180, 255, 30);
    color_ranges.emplace(colors::BLACK,tempColor);

    tempColor.low=cv::Scalar(0, 0, 231);
    tempColor.high=cv::Scalar(180, 2, 255);
    color_ranges.emplace(colors::WHITE,tempColor);

    tempColor.low=cv::Scalar(159, 3, 70);
    tempColor.high=cv::Scalar(9, 255, 255);
    color_ranges.emplace(colors::RED,tempColor);

    tempColor.low=cv::Scalar(36, 50, 70);
    tempColor.high=cv::Scalar(89, 255, 255);
    color_ranges.emplace(colors::GREEN,tempColor);

    tempColor.low=cv::Scalar(90, 50, 70);
    tempColor.high=cv::Scalar(128, 255, 255);
    color_ranges.emplace(colors::BLUE,tempColor);

    tempColor.low=cv::Scalar(25, 50, 70);
    tempColor.high=cv::Scalar(35, 255, 255);
    color_ranges.emplace(colors::YELLOW,tempColor);

    tempColor.low=cv::Scalar(129, 50, 70);
    tempColor.high=cv::Scalar(158, 255, 255);
    color_ranges.emplace(colors::PURPLE,tempColor);

    tempColor.low=cv::Scalar(10, 50, 70);
    tempColor.high=cv::Scalar(24, 255, 255);
    color_ranges.emplace(colors::ORANGE,tempColor);

    tempColor.low=cv::Scalar(0, 0, 40);
    tempColor.high=cv::Scalar(180, 18, 230);
    color_ranges.emplace(colors::GRAY,tempColor);


    cv::Rect led_box;

    led_box.width = kilobot->box.width*0.2;
    led_box.height = kilobot->box.height*0.2;
    led_box.x = kilobot->led_pos.x-led_box.width/2;
    led_box.y = kilobot->led_pos.y-led_box.height/2;

    if (led_box.x < 0 || led_box.y < 0){
        led_box.x = 0;
        led_box.y = 0;
    }
    if (0 <= led_box.x
        && 0 <= led_box.width
        && led_box.x + led_box.width <= extracted_kilobot.cols
        && 0 <= led_box.y
        && 0 <= led_box.height
        && led_box.y + led_box.height <= extracted_kilobot.rows)
    {
        cv::Mat extracted_led = cv::Mat(extracted_kilobot.clone(), led_box);
        if (kilobot->id == 2)
            cv::imshow("Ledka", extracted_led);
        cv::cvtColor(extracted_led,extracted_led,COLOR_BGR2HSV);

        for (const auto &[ color, color_HSV_limit] : color_ranges){
            cv::Mat col_mask;
            if (color == RED)
            {
                cv::inRange(extracted_led, color_HSV_limit.low, cv::Scalar (180, 255, 255), col_mask);
                colorValues[color] = cv::countNonZero(col_mask);
                cv::Mat colMaksTmp;
                cv::inRange(extracted_led, cv::Scalar (0,50,70), color_HSV_limit.high, colMaksTmp);
                colorValues[color] += cv::countNonZero(colMaksTmp);
            }
            else
            {
                cv::inRange(extracted_led,color_HSV_limit.low,color_HSV_limit.high,col_mask);
                colorValues[color] = cv::countNonZero(col_mask);
            }
        };

        int max = 0;
        int max_index = 0;

        for (int i = 0; i < 9; i++){
            if (colorValues[i]>= max)
            {
                max = colorValues[i];
                max_index = i;
            }
            if (kilobot->id == 2)
                std::cout << colorValues[i] << ", ";
        }

        if (kilobot->id == 2){
            std::cout << "max: " <<  max_index << std::endl;
        }

        kilobot->led_col = max_index;
        if (kilobot->id == 2){
            cout << "kilobot ID: " << kilobot->id <<"\t primary color: " << kilobot->led_col << std::endl;
            for (auto c: colorValues)
                cout << c << ",";
            std::cout << std::endl;
        }
    }
    else
    {
        cout << "out of box \n";
    }
}
void process_kilobot(cv::Mat frame, std::shared_ptr<Kilobot_detection_library::Kilobot> kilobot){

    std::vector<KeyPoint> blob_keypoints;

    if (0 <= kilobot->box.x
        && 0 <= kilobot->box.width
        && kilobot->box.x + kilobot->box.width <= frame.cols
        && 0 <= kilobot->box.y
        && 0 <= kilobot->box.height
        && kilobot->box.y + kilobot->box.height <= frame.rows)
    {

        cv::Mat extracted_kilobot = cv::Mat(frame.clone(), kilobot->box);
        cv::Mat extracted_kilobot_col = extracted_kilobot.clone();
        detect_LED(extracted_kilobot,kilobot);
        LED_color(extracted_kilobot_col,kilobot);

    }
    else
    {
        cout << "out_of_range \n";
    }


//TODO: angle calculation

}
cv::Scalar id_to_color (int id){

    switch (id) {
        case 0:
            return {0, 0, 0};
            break;
        case 1:
            return {255, 255, 255};
            break;
        case 2:
            return {0, 0, 255};
            break;
        case 3:
            return {0, 255, 0};
            break;
        case 4:
            return {255, 0, 0};
            break;
        case 5:
            return {0, 255, 255};
            break;
        case 6:
            return {83, 53, 149};
            break;
        case 7:
            return {51, 87, 255};
            break;
        case 8:
            return {128, 128, 128};
            break;
        default:
            return {30, 233, 40};
            break;
    };

}


int main(int argc, char** argv)
{
    // Creates YOLO detector and loads DNN model "best.onnx"
    YoloDetector detector;
    if (YD_RESULT::YD_OK != detector.LoadNet("../../../utils/best.onnx", false)){
        return -1;
    }

    // Open video "bodyguard.mp4"
//    cv::VideoCapture capture("../../../dataset/videa/search_and_rescue/Kilobot search and rescue.mp4");
    cv::VideoCapture capture("../../../utils/vid2.mp4");
    if (!capture.isOpened())
    {
        std::cerr << "Error opening video file\n";
        return -1;
    }

    // default values 
    int nms = 0;
    int score = 5;
    int conf = 7;
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
    std::vector<Kilobot> extracted_kilobots;

    cv::Mat frame;

    EuclidianTracker tracker;

    SimpleTracker tr;

    while (true) {
        // read frame
        capture.read(frame);
        cv::Mat scaled;
        cv::resize(frame,scaled,cv::Size(frame.cols*3,frame.rows*3));

//        scaled.copyTo(frame(cv::Rect(0,0,frame.cols,frame.rows)));
        frame = cv::Mat(scaled.clone(), cv::Rect(1300,1300,frame.cols,frame.rows));

        if (frame.empty()) {
            std::cout << "End of stream\n";
            break;
        }

        // detect Kilobots
        auto start = std::chrono::steady_clock::now();
        detector.Detect(frame, output, (float) (nms + 1) / 10, (float) (conf + 1) / 10, (float) (score + 1) / 10);
        auto stop = std::chrono::steady_clock::now();

        tr.processKilobots(output);

        for ( auto &[ID, kilobot, path] : tr.getKilobots()) {

           /* if (kilobot->id >25){
                break;
            }*/
            process_kilobot(frame, kilobot);


            cv::line(frame, kilobot->center(), cv::Point(kilobot->box.x + kilobot->led_pos.x, kilobot->box.y + kilobot->led_pos.y),
                     id_to_color(kilobot->led_col), 3);
        }



        tr.drawKilobots(frame);

        // show output
        imshow("output", frame);

        if (cv::waitKey(1) != -1) {
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