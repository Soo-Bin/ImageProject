/*
image_out3.bmp
image_out5.bmp
.
.
.
image_out11.bmp
과제로 해오기
가장자리는 그냥 copy 나머지는 평균을 내서 copy
*/
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
void main()
{
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height);
	int** image_out = (int**)IntAlloc2(width, height); // image_out[height][width]
	int x0 = 384;
	int y0 = 512;
	int r = 50;

	CopyImage(image, width, height, image_out);

	for (int y = 0; y<height; y++) {
		for (int x = 0; x<width; x++) {
			if (((x - x0)*(x - x0) + (y - y0)*(y - y0)) < r*r) {
				image_out[y][x] = 0;
			}
		}
	}
	/*
	for(int i=0;i<height;i++)
	for(int j=0;j<width;j++){
	int x = image[i][j];
	int y = (int)(255.0 * sin(PI/2.0 * x / 255.0));
	image_out[i][j]  = y;


	}
	}*/
	WriteImage("image_out.jpg", image_out, width, height);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width, height);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width, height);

}
