#include "kilolib_functions.h"
#include "camera_calibration.h"

using namespace cv;
using namespace std;
using namespace kilolib;

int main() {
	int percentage = 50;
	int thresh = 120;

	namedWindow("Tracks", WINDOW_AUTOSIZE);
	createTrackbar("percentage", "Tracks", &percentage, 100);
	createTrackbar("threshold", "Tracks", &thresh, 255);

	Mat frame;

	while (true) {

		if (percentage == 0)
			percentage = 1;

		if (thresh == 0)
			thresh = 1;

		centerKilobotDetection(frame, "images/test.png", percentage, thresh, true);

		imshow("output", frame);

		if (waitKey(1) != -1)
		{
			std::cout << "finished by user\n";
			break;
		}
	}

	return 0;
}