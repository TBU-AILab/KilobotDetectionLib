#include "euclidian_tracker.h"

//using namespace cv;

#include "kilolib_functions.h"

namespace kilolib {
    EuclidianTracker::EuclidianTracker(int lastId, int maxDist, int maxUndetected)
        : _maxDist(maxDist), _maxUndetected(maxUndetected), _lastId(lastId)
    {
    };

    void EuclidianTracker::Track(Mat frame, std::vector<Kilobot>& output, std::vector<Kilobot>& prevOutput, double LEDthreshVal, double LEDPercentageVal)
    {
        std::vector<Kilobot> undetected;

        for (int o = 0; o < output.size(); ++o)
        {
            auto detection = output[o];

            auto box = detection.box;

            //auto classId = detection.class_id;

            _findObject(frame, output[o], prevOutput, LEDthreshVal, LEDPercentageVal);

            // pokud jsem ho nena�el, pod�v�m se je�t� do nedetekovan�ch
            if (output[o].id == -1) {
                // projdu v�echny nedetekovan� kiloboty a pokus�m se je naj�t 
                _findObject(frame, output[o], _prevUndetected, LEDthreshVal, LEDPercentageVal);

                // pokud jsem ho nena�el ani v nedetekovan�ch, tak mu vygeneruju nov� ID
                if (output[o].id == -1) {
                    output[o].id = _lastId;
                    _lastId++;
                }
            }

            //for (Point p : output[i].trajectory)
            //    circle(frame, p, 2, output[i].color, -1);

            // ozna�en� bounding boxu kilobota
            //cv::rectangle(frame, box, Scalar(0, 255, 0), 1);
        }

        // pod�v�m se, jestli jsem detekoval v�echny kiloboty z p�edchoz� iterace
        for (int po = 0; po < prevOutput.size(); po++) {
            if (prevOutput[po].id != -1)
                undetected.push_back(prevOutput[po]);
        }

        // pod�v�m se, jestli jsem detekoval v�echny d��ve nedetekovan� kiloboty
        for (int pu = 0; pu < _prevUndetected.size(); pu++) {
            if (_prevUndetected[pu].id != -1)
                undetected.push_back(_prevUndetected[pu]);
        }

        _prevUndetected.clear();

        for (int u = 0; u < undetected.size(); u++) {
            undetected[u].undetected++;

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

        // najdu st�ed hledan�ho kilbota
        int x = box.br().x - box.width / 2;
        int y = box.br().y - box.height / 2;

        double minDistance = _maxDist;
        int index = -1; // defaultn� nastav�m index na -1, co� znamen�, �e jsem ��dn� nena�el

        for (int i = 0; i < objects.size(); i++) {
            // pokud se jedn� u� o sp�rovan� objekt, tak ho nem��u vz�t podruh�
            if (objects[i].id == -1)
                continue;

            Rect prevR = objects[i].box;
            Point p = Point(prevR.tl().x + prevR.width / 2, prevR.tl().y + prevR.height / 2);
            Point q = Point(box.tl().x + box.width / 2, box.tl().y + box.height / 2);

            double distance = sqrt(abs(pow(q.x - p.x, 2) + pow(q.y - p.y, 2)));

            if (distance < minDistance) {
                minDistance = distance;
                index = i;
            }
        }

        if (index != -1) {
            obj.id = objects[index].id; //zkop�ruju id
            objects[index].id = -1; // ozna��m objekt jako ji� sp�rovan�
            obj.trajectory = objects[index].trajectory;
            obj.trajectory.push_back(Point(x, y));

            obj.led = objects[index].led;
            obj.led.push_back(findLED(frame, obj.box, LEDthreshVal, LEDPercentageVal));

            obj.color = objects[index].color;
            obj.LEDcolor = objects[index].LEDcolor;
        }

        if (obj.id != -1)
            cv::putText(frame, to_string(obj.id), Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 2);
    }
}