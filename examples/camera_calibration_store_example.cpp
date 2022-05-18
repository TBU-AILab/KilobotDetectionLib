#include "camera_calibration.h"

using namespace cv;
using namespace std;
using namespace kilolib;

int main()
{
    // Creating vector to store vectors of 3D points for each checkerboard image
    vector<vector<Point3f>> objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    vector<vector<Point2f>> imgpoints;

    cv::Mat cameraMatrix, distCoeffs, R, T;

    Size size;

    if (CalculatePoints("images/", ".jpg", 6, 7, objpoints, imgpoints, size)) {
        /*
         * Performing camera calibration by
         * passing the value of known 3D points (objpoints)
         * and corresponding pixel coordinates of the
         * detected corners (imgpoints)
        */
        calibrateCamera(objpoints, imgpoints, size, cameraMatrix, distCoeffs, R, T);

        std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
        std::cout << "distCoeffs : " << distCoeffs << std::endl;
        std::cout << "Rotation vector : " << R << std::endl;
        std::cout << "Translation vector : " << T << std::endl;

        Mat original;
        Mat un;

        StoreCalibration("calibration.xml", cameraMatrix, distCoeffs);

        if (cameraMatrix.empty() || distCoeffs.empty()) {
            std::cout << "the matrices were not loaded correctly" << std::endl;
            return 0;
        }

        while (true) {

            original = imread("images/11.jpg");

            undistort(original, un, cameraMatrix, distCoeffs);

            imshow("original", original);
            imshow("undistorted", un);

            if (waitKey(1) != -1)
            {
                std::cout << "finished by user\n";
                break;
            }
        }
    }
    else
        std::cout << "Not enough good images for camera calibration.";

    waitKey(0);

    return 0;
}