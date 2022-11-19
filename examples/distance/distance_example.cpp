/*****************************************************************//**
 * @file   distance_example.cpp
 * @brief  Example that detects Kilobot in the center of image and display distance from camera.
 *		   Detection is done by findContours and expects no other items except Kilobot.
 *
 * @author Petr Svoboda
 * @date   May 2022
***********************************************************************/

#include "kilolib_functions.h"
#include "camera_calibration.h"

using namespace cv;
using namespace std;
using namespace Kilobot_detection_library;

int main() {
	// values that works with "images/test.png"
	int percentage = 20;
	int thresh = 80;

	// Creates window with TrackBars
	// trackbar percentage - Part of the Image around the center, where Kilobot should be placed.
	// trackbar thresh - Threshold value for findContours function.
	namedWindow("Tracks", WINDOW_AUTOSIZE);
	createTrackbar("percentage", "Tracks", &percentage, 100);
	createTrackbar("threshold", "Tracks", &thresh, 255);

	Mat frame;

	while (true) {

		// Part of the image should be at least 1% - to avoid crashes
		if (percentage == 0)
			percentage = 1;

		// Detect Kilobot in the center of "images/test.png"
		// and discard data outside center of the image except of percentage
		// then found contours with value of thresh.
		// If last param == true measureDisatnce from camera and display it
		centerKilobotDetection(frame, "images/test.png", percentage, thresh, true);

		// Display image
		imshow("output", frame);

		if (waitKey(1) != -1)
		{
			std::cout << "finished by user\n";
			break;
		}
	}

	return 0;
}