//
// Created by Peter Janků on 13.10.2022.
//

#ifndef KILOLIB_SIMPLETRACKER_H
#define KILOLIB_SIMPLETRACKER_H

#include <list>
#include <tuple>
#include <deque>
#include <iostream>
#include "kilobot.h"
#include "opencv2/opencv.hpp"

const cv::Scalar colors[] = {
        {255, 204, 153},
        {204, 153, 255},
        {255, 153, 204},
        {153, 204, 255},
        {255, 102, 51},
        {204, 204, 51},
        {255, 102, 51},
        {204, 204, 51},
        {0,   204, 153},
        {255, 204, 0},
        {255, 153, 0}
};


class SimpleTracker {
public:
    auto processKilobot(kilolib::Kilobot &kilobot, int pathLimit = 0) {
        bool found = false;
        for (auto &[rtl, k, path]: kilobots) {
            auto diff = k->box & kilobot.box;
            if ((diff.area() / (((float) k->box.area() + kilobot.box.area()) / 2)) > 0.6) {
                k->box = kilobot.box;
                if (rtl > 0) rtl = 0;

                auto last = path.back();
                auto centerDiff = last - kilobot.center();
                if ((abs(centerDiff.x) + abs(centerDiff.y)) > 2) {
                    path.push_back(kilobot.center());
                    if (pathLimit > 0)
                        if (path.size() > pathLimit) path.pop_front();
                }

                found = true;
                break;
            }
        }
        if (!found) {
            kilobot.id = lastID++;
            std::deque<Point> path;
            //std::cout << "Path size: " << path.size() << std::endl;
            path.push_back(kilobot.center());
            kilobots.emplace_back(0, std::make_shared<kilolib::Kilobot>(kilobot), path);
        }
    }

    auto increaseRTL() {
        for (auto &[rtl, k, path]: kilobots) {
            rtl++;
            if (rtl > 10) {
                std::cout << k->id << " lost" << std::endl;
            }
        }
        kilobots.remove_if([](const auto &el) {
            auto &[rtl, k, path] = el;
            if (rtl > 5) return true;
            else return false;
        });
    }

    auto processKilobots(std::vector<kilolib::Kilobot> &input) {
        for (auto &k: input) {
            processKilobot(k);
        }
        increaseRTL();
    }


    auto drawKilobots(cv::Mat frame) {
        for (auto &[rtl, kilo, path]: kilobots) {
            cv::putText(frame, std::to_string(kilo->id), Point(kilo->box.x, kilo->box.y), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(255, 0, 0), 2);
            cv::putText(frame, std::to_string(rtl), Point(kilo->box.x, kilo->box.y - 20), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(0, 0, 255), 2);
            if (rtl > 5) {
                cv::circle(frame, kilo->center(), kilo->box.width / 2, cv::Scalar(0, 0, 255));
                //cv::rectangle(frame, kilo->box, cv::Scalar(0, 0, 255));
            } else {
                cv::circle(frame, kilo->center(), kilo->box.width / 2, cv::Scalar(0, 255, 0));
                //cv::rectangle(frame, kilo->box, cv::Scalar(0, 255, 0));
            }
            if (path.size() > 1) {
                int step = 1;
                if (path.size() > 10) step = 5;
                auto first = path.front();
                for (int i = 1; i < path.size(); i += step) {
                    auto next = path[i];
                    cv::line(frame, first, next, colors[kilo->id % 11], 2);
                    first = next;
                }

            }

        }

    }

    void testMethod() {

        for (auto p: points) {

        }

    }

    std::deque<Point> points;
protected:
    std::list<std::tuple<int, std::shared_ptr<kilolib::Kilobot>, std::deque<Point>>> kilobots;
    int lastID = 0;
};


#endif //KILOLIB_SIMPLETRACKER_H
