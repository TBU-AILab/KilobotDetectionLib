#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

namespace kilolib {

    struct Kilobot
    {
        float confidence; // "míra jistoty" detekce z neuronové sítì
        Rect box; // ohranièení kilobota
        int id; // id konkrétního kilobota
        int undetected; // poèet snímkù na kterých nebyl detekován
        std::vector<Point> led; // støed detekované LED
        std::vector<Point> trajectory; // zaznamenaná trajektorie
        Scalar color; // barva pro vykreslení
        Scalar LEDcolor; // barva pro vykreslení
    };
}