#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

#define PI 3.14159265359

int** IntAlloc2(int width, int height)
{
	int** tmp;
	tmp = (int**)calloc(height, sizeof(int*));
	for (int i = 0; i<height; i++)
		tmp[i] = (int*)calloc(width, sizeof(int));
	return(tmp);
}

void IntFree2(int** image, int width, int height)
{
	for (int i = 0; i<height; i++)
		free(image[i]);

	free(image);
}

int** ReadImage(char* name, int* width, int* height)
{
	Mat img = imread(name, IMREAD_GRAYSCALE);
	int** image = (int**)IntAlloc2(img.cols, img.rows);

	*width = img.cols;
	*height = img.rows;

	for (int i = 0; i<img.rows; i++)
		for (int j = 0; j<img.cols; j++)
			image[i][j] = img.at<unsigned char>(i, j);

	return(image);
}

void WriteImage(char* name, int** image, int width, int height)
{
	Mat img(height, width, CV_8UC1);
	for (int i = 0; i<height; i++)
		for (int j = 0; j<width; j++)
			img.at<unsigned char>(i, j) = (unsigned char)image[i][j];

	imwrite(name, img);
}

void ImageShow(char* winname, int** image, int width, int height)
{
	Mat img(height, width, CV_8UC1);
	for (int i = 0; i<height; i++)
		for (int j = 0; j<width; j++)
			img.at<unsigned char>(i, j) = (unsigned char)image[i][j];
	imshow(winname, img);

}

void CopyImage(int** src, int width, int height, int** dst)
{
	for (int y = 0; y<height; y++)
		for (int x = 0; x<width; x++)
			dst[y][x] = src[y][x];
}

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

int main()
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