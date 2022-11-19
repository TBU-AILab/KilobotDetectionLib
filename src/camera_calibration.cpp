#include "camera_calibration.h"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

namespace Kilobot_detection_library {

    bool CalculatePoints(string folderPath, string suffix, int cbWidth, int cbHeight,
        vector<vector<Point3f>>& objPoints, vector<vector<Point2f>>& imgPoints, Size& size) {

        // Defining the world coordinates for 3D points
        vector<Point3f> objp;
        for (int i = 0; i < cbHeight; i++)
        {
            for (int j = 0; j < cbWidth; j++)
                objp.push_back(Point3f(j, i, 0));
        }

        // Extracting path of individual image stored in a given directory
        vector<String> images;
        // Path of the folder containing checkerboard images

        stringstream stream;
        stream << "./" << folderPath << "*" << suffix;

        string path = stream.str();

        // list all filenames to images vector
        glob(path, images);

        Mat frame, gray;
        // vector to store the pixel coordinates of detected checker board corners 
        vector<Point2f> corner_pts;
        bool success;

        // Looping over all the images in the directory
        for (int i = 0; i < images.size(); i++)
        {
            std::cout << "processing img: " << i + 1 << std::endl;

            frame = imread(images[i]);
            cvtColor(frame, gray, COLOR_BGR2GRAY);

            // Finding checker board corners
            // If desired number of corners are found in the image then success = true  
            success = findChessboardCorners(gray, Size(cbWidth, cbHeight), corner_pts);

            /*
             * If desired number of corner are detected,
             * we refine the pixel coordinates and display
             * them on the images of checker board
            */
            if (success)
            {
                std::cout << "accepted" << std::endl;

                TermCriteria criteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);

                // refining pixel coordinates for given 2d points.
                cornerSubPix(gray, corner_pts, Size(11, 11), Size(-1, -1), criteria);

                // Displaying the detected corner points on the checker board
                drawChessboardCorners(frame, Size(cbWidth, cbHeight), corner_pts, success);

                objPoints.push_back(objp);
                imgPoints.push_back(corner_pts);

                //imshow("img", frame);
                //waitKey(0);
            }
        }

        if (!gray.empty())
            size = gray.size();

        if (objPoints.size() > 2) // minimum size needed for calculation
            return true;

        return false;
    }

    void StoreCalibration(string fileName, Mat cameraMatrix, Mat distCoeffs) {
        FileStorage fs(fileName, FileStorage::WRITE);

        fs << "Camera_matrix" << cameraMatrix;
        fs << "Distortion_coeficients" << distCoeffs;
    }

    void LoadCalibration(string fileName, Mat& cameraMatrix, Mat& distCoeffs) {
        FileStorage fs(fileName, FileStorage::READ);

        fs["Camera_matrix"] >> cameraMatrix;
        fs["Distortion_coeficients"] >> distCoeffs;
    }

}
