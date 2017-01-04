#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

void ConvertMat2Image(Mat& img_mat, int** image_out)
{
	Mat img_mat_gray;

	if (img_mat.channels() != 1) {
		cvtColor(img_mat, img_mat_gray, COLOR_BGR2GRAY);
		//printf("\n BGR to gray was converted!");
	}
	else
		img_mat_gray = img_mat.clone();

	for (int i = 0; i < img_mat.rows; i++) for (int j = 0; j < img_mat.cols; j++)
		image_out[i][j] = img_mat_gray.at<unsigned char>(i, j);
}

int main_LifeCam()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		int width = frame.cols;
		int height = frame.rows;
		int** image = (int**)IntAlloc2(width, height);
		ConvertMat2Image(frame, image);

		ImageShow("image", image, width, height);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor

	return 0;

}
