//
// Created by Peter Janků on 13.10.2022.
//

#ifndef KILOLIB_SIMPLETRACKER_H
#define KILOLIB_SIMPLETRACKER_H

#include <list>
#include <tuple>
#include <deque>
#include <array>
#include <iostream>
#include "kilobot.h"
#include "opencv2/opencv.hpp"

namespace kilolib {

    const std::array<cv::Scalar, 11> colors = {
            cv::Scalar({255, 204, 153}),
            cv::Scalar({204, 153, 255}),
            cv::Scalar({255, 153, 204}),
            cv::Scalar({153, 204, 255}),
            cv::Scalar({255, 102, 51}),
            cv::Scalar({204, 204, 51}),
            cv::Scalar({255, 102, 51}),
            cv::Scalar({204, 204, 51}),
            cv::Scalar({0, 204, 153}),
            cv::Scalar({255, 204, 0}),
            cv::Scalar({255, 153, 0})
    };


    class SimpleTracker {
    public:
        void processKilobot(kilolib::Kilobot &kilobot, int pathLimit = 0);

        void increaseRTL(int maxRTL = 5);

        void processKilobots(std::vector<kilolib::Kilobot> &input);


        void drawKilobots(cv::Mat frame);

        auto &getKilobots() { return kilobots; }


    private:
        std::list<std::tuple<int, std::shared_ptr<kilolib::Kilobot>, std::deque<Point>>> kilobots;
        int lastID = 0;
    };
}

#endif //KILOLIB_SIMPLETRACKER_H
