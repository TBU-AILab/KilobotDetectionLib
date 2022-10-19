//
// Created by Peter Janků on 13.10.2022.
//

#include "simpletracker.h"
namespace kilolib {


    void SimpleTracker::increaseRTL(int maxRTL) {
        for (auto &[rtl, k, path]: kilobots) {
            rtl++;
        }
        kilobots.remove_if([maxRTL](const auto &el) {
            auto &[rtl, k, path] = el;
            if (rtl > maxRTL) return true;
            else return false;
        });
    }

    void SimpleTracker::processKilobots(std::vector<kilolib::Kilobot> &input) {
        for (auto &k: input) {
            processKilobot(k);
        }
        increaseRTL();
    }

    void SimpleTracker::drawKilobots(cv::Mat frame) {
        for (auto &[rtl, kilo, path]: kilobots) {
            cv::putText(frame, std::to_string(kilo->id), Point(kilo->box.x, kilo->box.y), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(255, 0, 0), 2);
            cv::putText(frame, std::to_string(rtl), Point(kilo->box.x, kilo->box.y - 20), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(0, 0, 255), 2);
            if (rtl > 5) {
                cv::circle(frame, kilo->center(), kilo->box.width / 2, cv::Scalar(0, 0, 255));
            } else {
                cv::circle(frame, kilo->center(), kilo->box.width / 2, cv::Scalar(0, 255, 0));
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

    void SimpleTracker::processKilobot(kilolib::Kilobot &kilobot, int pathLimit) {
        bool found = false;
        for (auto &[rtl, k, path]: kilobots) {
            auto diff = k->box & kilobot.box;
            if (((float) diff.area() / ((float) (k->box.area() + kilobot.box.area()) / 2)) > 0.6) {
                k->box = kilobot.box;
                if (rtl > 0) rtl = 0;

                auto last = path.back();
                if (auto centerDiff = last - kilobot.center(); (abs(centerDiff.x) + abs(centerDiff.y)) > 2) {
                    path.push_back(kilobot.center());
                    if ((pathLimit > 0) && (path.size() > pathLimit)) path.pop_front();
                }

                found = true;
                break;
            }
        }
        if (!found) {
            kilobot.id = lastID++;
            std::deque<Point> path;
            path.push_back(kilobot.center());
            kilobots.emplace_back(0, std::make_shared<kilolib::Kilobot>(kilobot), path);
        }
    }
}