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
int main_test(){ 
	VideoCapture cap(0); // open the default camera 
	if (!cap.isOpened())  // check if we succeeded  
		return -1;
	Mat edges; 
	namedWindow("edges", 1); 
	for (;;) {  
		Mat frame;
		cap >> frame; // get a new frame from camera  
		cvtColor(frame, edges, COLOR_BGR2GRAY);  
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		Mat dx, dy;
		Sobel(edges, dx, CV_32F, 1, 0);
		Sobel(edges, dy, CV_32F, 0, 1);  
		vector<Vec4i> lines; 
		HoughLinesP(edges, lines, 1, CV_PI / 180, 100, 100, 10);
		Mat Blank(frame.rows, frame.cols, CV_8UC3, Scalar(0, 0, 0));  
		for (size_t i = 0; i < lines.size(); i++) {   
			Vec4i l = lines[i];  
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 2, CV_AA);   
			line(Blank, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 2, CV_AA); 
		} 
		imshow("edges", Blank);
		if (waitKey(30) >= 0) break; 
	} // the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
