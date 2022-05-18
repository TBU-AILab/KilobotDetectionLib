#include "camera_calibration.h"

using namespace cv;
using namespace std;
using namespace kilolib;

int main()
{
    vector<vector<Point3f>> objpoints;

    vector<vector<Point2f>> imgpoints;

    cv::Mat cameraMatrix, distCoeffs;

    Mat original;
    Mat un;

    LoadCalibration("calibration.xml", cameraMatrix, distCoeffs);

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

    waitKey(0);

    return 0;
}
