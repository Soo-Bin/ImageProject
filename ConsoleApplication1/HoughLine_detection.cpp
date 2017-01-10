#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
int main_HoughLine()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat dst;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat src;
		cap >> src; // get a new frame from camera
		cvtColor(src, dst, COLOR_BGR2GRAY);
		GaussianBlur(dst, dst, Size(7, 7), 1.5, 1.5);
		Canny(dst, dst, 30, 100, 3);
		imshow("canny", dst);

		vector<Vec4i> lines;
		lines.reserve(1000);

		HoughLinesP(dst, lines, 1, CV_PI / 180, 10, 30, 10);

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			Point p1, p2;
			p1 = Point(l[0], l[1]);
			p2 = Point(l[2], l[3]);

			line(src, p1, p2, Scalar(0, 0, 255), 2);
		}

		imshow("result", src);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
