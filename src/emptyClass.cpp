#include "emptyClass.h"

using namespace cv;

EmptyClass::EmptyClass()
{
	Mat mat;
	mat = Mat::zeros(Size(10,10), CV_8UC3);
	Mat mat2;
	mat.copyTo(mat2);
}
