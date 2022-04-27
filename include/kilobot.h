#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

namespace kilolib {

    struct Kilobot
    {
        float confidence; // "m�ra jistoty" detekce z neuronov� s�t�
        Rect box; // ohrani�en� kilobota
        int id; // id konkr�tn�ho kilobota
        int undetected; // po�et sn�mk� na kter�ch nebyl detekov�n
        std::vector<Point> led; // st�ed detekovan� LED
        std::vector<Point> trajectory; // zaznamenan� trajektorie
        Scalar color; // barva pro vykreslen�
        Scalar LEDcolor; // barva pro vykreslen�
    };
}