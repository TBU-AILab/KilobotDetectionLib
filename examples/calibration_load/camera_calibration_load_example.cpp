/*****************************************************************//**
 * @file   camera_calibration_load_example.cpp
 * @brief  Example that loads calibration parameters from file and applies them on image.
 *
 * @author Petr Svoboda
 * @date   May 2022
***********************************************************************/

#include "camera_calibration.h"

using namespace cv;
using namespace std;
using namespace Kilobot_detection_library;

int main()
{
    // Creating vector to store vectors of 3D points for each checkerboard image
    vector<vector<Point3f>> objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    vector<vector<Point2f>> imgpoints;

    cv::Mat cameraMatrix, distCoeffs;

    Mat original;
    Mat un;

    // load calibration.xml file and store data into camera matrix and distoriton coefficients matrixes
    LoadCalibration("calibration.xml", cameraMatrix, distCoeffs);

    if (cameraMatrix.empty() || distCoeffs.empty()) {
        std::cout << "the matrices were not loaded correctly" << std::endl;
        return 0;
    }

    while (true) {
        // read test frame
        original = imread("images/11.jpg");

        // undistort view
        undistort(original, un, cameraMatrix, distCoeffs);

        // show original and undistorted img
        imshow("original", original);
        imshow("undistorted", un);

        if (waitKey(1) != -1)
        {
            std::cout << "finished by user\n";
            break;
        }
    }

    waitKey(0);

    return 0;
}