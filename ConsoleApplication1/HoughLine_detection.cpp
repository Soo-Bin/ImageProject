#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

#define PI 3.14159265359

using namespace cv;
using namespace std;

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

void HoughCalc(vector<Vec4i> lines, Mat frame_out)
{
	Vec4i params;
	float rho, theta;
	float c, s;
	float x0, y0;
	for (int i = 0; i < lines.size(); i++) {
		params = lines[i];

		rho = params[0];
		theta = params[1];
		printf("lines[%2d]=(rho,theta) = (%f, %f)\n", i, rho, theta);

		c = cos(theta);
		s = sin(theta);
		x0 = rho*c;
		y0 = rho*s;

		Point pt1, pt2;
		pt1.x = cvRound(x0 + 1000 * (-s));
		pt1.y = cvRound(y0 + 1000 * (c));

		pt2.x = cvRound(x0 - 1000 * (-s));
		pt2.y = cvRound(y0 - 1000 * (c));
		line(frame_out, pt1, pt2, Scalar(0, 0, 255), 2);
	}
}
int main_HoughLine()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		Mat edges;
		cap >> frame; // get a new frame from camera
		Canny(frame, edges, 50, 100);
		imshow("edges", edges);

		vector<Vec4i> lines;
		HoughLines(edges, lines, 1, CV_PI / 180.0, 100);
		
		Mat frame_out(frame.size(), CV_8UC3);
		cvtColor(frame, frame_out, COLOR_GRAY2BGR);

		HoughCalc(lines, frame_out);
		//imshow("frame_out", frame_out);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor

	return 0;

}
