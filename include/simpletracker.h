//
// Created by Peter Janků on 13.10.2022.
//

#ifndef KILOLIB_SIMPLETRACKER_H
#define KILOLIB_SIMPLETRACKER_H

#include <list>
#include <tuple>
#include <iostream>
#include "kilobot.h"
#include "opencv2/opencv.hpp"

class SimpleTracker {
public:
    auto processKilobot(kilolib::Kilobot &kilobot) {
        bool found = false;
        for (auto &[rtl, k]: kilobots) {
            auto diff = k->box & kilobot.box;
            if ((diff.area() / (float) k->box.area()) > 0.8) {
                k->box = kilobot.box;
                if (rtl > 0) rtl--;
                found = true;
                break;
            }
        }
        if (!found) {
            kilobot.id = lastID++;
            kilobots.push_back({0, std::shared_ptr<kilolib::Kilobot>(new kilolib::Kilobot(kilobot))});
        }
    }

    auto increaseRTL() {
        for (auto &[rtl, k]: kilobots) {
            rtl++;
            if (rtl > 5) std::cout << k->id << " lost" << std::endl;
        }
    }

    auto processKilobots(std::vector<kilolib::Kilobot> &input) {
        for (auto &k: input) {
            processKilobot(k);
        }
        increaseRTL();
    }


    auto drawKilobots(cv::Mat frame) {
        for (const auto &[rtl, kilo]: kilobots) {
            cv::putText(frame, std::to_string(rtl), Point(kilo->box.x, kilo->box.y), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(255, 0, 0), 2);
            if (rtl > 5) {
                cv::rectangle(frame, kilo->box, cv::Scalar(0, 0, 255));
            } else {
                cv::rectangle(frame, kilo->box, cv::Scalar(0, 255, 0));
            }
        }

    }

protected:
    std::list<std::tuple<int, std::shared_ptr<kilolib::Kilobot>>> kilobots;
    int lastID = 0;
};


#endif //KILOLIB_SIMPLETRACKER_H
