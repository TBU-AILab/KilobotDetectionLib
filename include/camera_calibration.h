#pragma once

/*****************************************************************//**
 * @file   camera_calibration.h
 * @brief  Helpful functions for camera calibration.
 *
 * @author Petr Svoboda
 * @date   May 2022
***********************************************************************/

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace kilolib {

    /**
     * @brief Loads all image with expected suffix from folder and calculates points needed for camera calibration.
     * 
     * @param folderPath Path to folder containing images for calibration. (Should end with character / )
     * @param suffix Image files suffix. (For example .jpg)
     * @param cbWidth Chessboard width.
     * @param cbHeight Chessboard height.
     * @param objPoints Vector to store vectors of 3D points for each checkerboard image.
     * @param imgPoints Vector to store vectors of 2D points for each checkerboard image.
     * @param size Calibration image size.
     * 
     * @return Returns True if calculation was successful
    */
	bool CalculatePoints(string folderPath, string suffix, int cbWidth, int cbHeight,
		vector<vector<Point3f>>& objPoints, vector<vector<Point2f>>& imgPoints, Size& size);

    /**
     * @brief Stores a calibration file.
     * 
     * @param fileName Path to file.
     * @param cameraMatrix Calibration matrix.
     * @param distCoeffs Distortion coefficients.
    */
	void StoreCalibration(string fileName, Mat cameraMatrix, Mat distCoeffs);

    /**
     * @brief Loads a calibration file.
     * 
     * @param fileName Path to file.
     * @param cameraMatrix Where to store calibration matrix.
     * @param distCoeffs Where to store distortion coefficients.
    */
	void LoadCalibration(string fileName, Mat& cameraMatrix, Mat& distCoeffs);
}