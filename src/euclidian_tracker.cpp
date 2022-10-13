#include "euclidian_tracker.h"

#include "kilolib_functions.h"

namespace kilolib {
    EuclidianTracker::EuclidianTracker(int lastId, int maxDist, int maxUndetected)
        : _maxDist(maxDist), _maxUndetected(maxUndetected), _lastId(lastId)
    {
    };

    void EuclidianTracker::Track(Mat frame, std::vector<Kilobot>& output, std::vector<Kilobot>& prevOutput, double LEDthreshVal, double LEDPercentageVal)
    {
        std::vector<Kilobot> undetected;
        // check every single Kilobot in vector of Kilobots found in current frame
        for (auto &kilobot: output) {
            //auto detection = output[o];
            auto box = kilobot.box;

            // try to identify Kilobot in last detections
            _findObject(frame, kilobot, prevOutput, LEDthreshVal, LEDPercentageVal);

            // if Kilobot wasn't found in previous detections
            if (kilobot.id == -1) {
                // check all Kilobots that weren't detected in previous iterations
                _findObject(frame, kilobot, _prevUndetected, LEDthreshVal, LEDPercentageVal);

                // if even than Kilobot wasn't found, generate new ID and assign it to Kilobot
                if (kilobot.id == -1) {
                    kilobot.id = _lastId;
                    _lastId++;
                }
            }
        }

        // take all undetected Kilobots from last iteration
        for (int po = 0; po < prevOutput.size(); po++) {
            if (prevOutput[po].id != -1)
                undetected.push_back(prevOutput[po]);
        }

        // take all undetected Kilobots from previous iterations
        for (int pu = 0; pu < _prevUndetected.size(); pu++) {
            if (_prevUndetected[pu].id != -1)
                undetected.push_back(_prevUndetected[pu]);
        }

        _prevUndetected.clear();

        // update number of undetected frames
        for (int u = 0; u < undetected.size(); u++) {
            undetected[u].undetected++;

            // if that number is lower than maximum allowed number of undetected frames keep Kilobot in memory, 
            // othervise it will be forgotten
            if (undetected[u].undetected < _maxUndetected)
                _prevUndetected.push_back(undetected[u]);
        }
    };

    void EuclidianTracker::Reset(int lastId, int maxDist, int maxUndetected)
    {
        _lastId = lastId;
        _maxDist = maxDist;
        _maxUndetected = maxUndetected;
        _prevUndetected.clear();
    }

    void EuclidianTracker::_findObject(Mat& frame, Kilobot& obj, std::vector<Kilobot>& objects, double LEDthreshVal, double LEDPercentageVal) {
        Rect box = obj.box;

        // determinate center of Kilobots bounding box
        int x = box.br().x - box.width / 2;
        int y = box.br().y - box.height / 2;

        double minDistance = _maxDist;
        int index = -1; // default index -1 means that Kilobot wasn't found

        for (int i = 0; i < objects.size(); i++) {
            // if id is already assigned, continue with another Kilobot
            if (objects[i].id == -1)
                continue;

            Rect prevR = objects[i].box;
            Point p = Point(prevR.tl().x + prevR.width / 2, prevR.tl().y + prevR.height / 2);
            Point q = Point(box.tl().x + box.width / 2, box.tl().y + box.height / 2);

            // calculation of euclidian distance between centers of current and previous detection
            double distance = sqrt(abs(pow(q.x - p.x, 2) + pow(q.y - p.y, 2)));

            // if distance is lower than previously lowest distance update index and distance
            if (distance < minDistance) {
                minDistance = distance;
                index = i;
            }
        }

        // if Kilobot was found in detections
        if (index != -1) {
            obj.id = objects[index].id; //copy his ID
            objects[index].id = -1; // assign -1 and tell algorithm that this Kilobot is already assigned
            obj.trajectory = objects[index].trajectory; // copy previous trajectory
            obj.trajectory.push_back(Point(x, y)); // add new trajectory point

            obj.led = objects[index].led; // copy previous LED detections
            obj.led.push_back(findLED(frame, obj.box, LEDthreshVal, LEDPercentageVal)); // add new LED center

            // copy colors
            obj.color = objects[index].color;
            obj.LEDcolor = objects[index].LEDcolor;
        }

        // paint ID inside Kilobot bounding box 
        if (obj.id != -1)
            cv::putText(frame, to_string(obj.id), Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 2);
    }
}