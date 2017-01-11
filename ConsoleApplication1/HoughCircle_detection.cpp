#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <iostream>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.14159265359

using namespace cv;
using namespace std;

int main_HoughCircle()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	for (;;)
	{
		Mat src, dst;
		cap >> src; // get a new frame from camera
		cvtColor(src, dst, COLOR_BGR2GRAY);
		GaussianBlur(dst, dst, Size(7, 7), 1.5, 1.5);
		Canny(dst, dst, 0, 100, 3);
					
		imshow("Canny", dst);

		vector<Vec3f> circles;
		circles.reserve(1000);

		vector<Vec4i> lines;
		lines.reserve(1000);

		HoughLinesP(dst, lines, 1, CV_PI / 180, 10, 30, 10);

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			Point p1, p2;
			p1 = Point(l[0], l[1]);
			p2 = Point(l[2], l[3]);

			line(dst, p1, p2, Scalar(0, 0, 0), 90);
		}

		HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 4, dst.rows/4, 10, 100, 0, 200);
		
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(src, center, radius, Scalar(0, 0, 255), 2);

		}
		
		imshow("result", src);
		
		if (waitKey(30) >= 0) break;

	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
