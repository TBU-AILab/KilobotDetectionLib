#include "kilolib_functions.h"

namespace kilolib{

    Point findLED(Mat frame, Rect box, double thresh, double minArea)
    {
        int x = -1;
        int y = -1;

        Mat kilobot = frame(box);

        // remove everything outside the Kilobot
        Mat mask = Mat::zeros(kilobot.rows, kilobot.cols, kilobot.type());

        double radius = kilobot.rows / 2;

        if (kilobot.rows > kilobot.cols)
            radius = kilobot.cols / 2;

        circle(mask, Point(kilobot.rows / 2, kilobot.cols / 2), radius, Scalar(255, 255, 255), FILLED);

        Mat res;
        bitwise_and(kilobot, mask, res);

        // remove everything around Kilobots center
        radius = res.size().width / 3;
        circle(res, Point(res.rows / 2, res.cols / 2), radius, Scalar(0, 0, 0), FILLED);

        Mat gray;
        cvtColor(res, gray, COLOR_BGR2GRAY);

        // apply thresholding - remove non-bright colors
        Mat th;
        threshold(gray, th, thresh, 255, THRESH_BINARY);

        // find biggest contour - biggest bright space
        vector<vector<Point>> contours;
        findContours(th, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        int biggestId = -1;
        int biggestSize = -1;
        Rect biggestBox;

        for (int c = 0; c < contours.size(); c++) {
            Rect cBox = boundingRect(contours[c]);

            double ledArea = (double)cBox.width * (double)cBox.height;

            if (ledArea > biggestSize) {
                biggestId = c;
                biggestSize = ledArea;
                biggestBox = cBox;
            }
        }

        if (biggestId != -1) {
            double kilobotArea = (double)th.rows * (double)th.cols;
            double ledArea = (double)biggestBox.width * (double)biggestBox.height;
            double percentage = (ledArea * 100) / kilobotArea;

            if (percentage > minArea) {
                x = box.tl().x + biggestBox.tl().x + biggestBox.width / 2;
                y = box.tl().y + biggestBox.tl().y + biggestBox.height / 2;
            }
        }

        return Point(x, y);
    }

    double measureDistance(Rect box, Mat frame, bool display, Scalar textColor)
    {

        if (box.empty()) {
            std::cout << "Unable to display distance, box is empty!" << std::endl;
            return -1;
        }

        if (display && frame.empty()) {
            std::cout << "Unable to display distance, frame is empty!" << std::endl;
            return -1;
        }

        //// determinate the distance
        double distance = kilobotSize * F / ((box.width + box.height) / 2);

        if (display) {

            // Create an output string stream
            std::ostringstream streamObj;
            // Set Fixed -Point Notation
            streamObj << std::fixed;
            // Set precision to 2 digits
            streamObj << std::setprecision(2);
            //Add double to stream
            streamObj << distance;
            // Get string from output string stream
            std::string strObj = streamObj.str();

            cv::putText(frame, strObj, box.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.4, textColor, 1);
        }

        return distance;
    }

}
