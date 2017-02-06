#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

#define FOCAL 0.5
#define Z 1

int** IntAlloc2(int width, int height)
{
	int** tmp;
	tmp = (int**)calloc(height, sizeof(int*));
	for (int i = 0; i<height; i++)
		tmp[i] = (int*)calloc(width, sizeof(int));
	return(tmp);
}
int*** IntAlloc3(int width, int height, int deep)
{
	int*** tmp;
	tmp= (int***)calloc(deep, sizeof(int**));
	
	for (int i = 0; i < deep; i++)
	{
		tmp[deep] = (int**)calloc(height, sizeof(int*));
		for (int j = 0; j < height; j++)
			tmp[deep][j] = (int*)calloc(width, sizeof(int));
		
	}
	return(tmp);
}
void IntFree2(int** image, int width, int height)
{
	for (int i = 0; i<height; i++)
		free(image[i]);

	free(image);
}
void IntFree3(int*** image, int width, int height, int deep)
{
	for (int i = 0; i < deep; i++)
	{
		free(image[deep]);
		for (int j = 0; j < height; j++)
			free(image[deep][j]);
	}
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
void CopyImage3(int** image, int height, int width, int*** img)
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			img[Z][y][x] = image[y][x];
}

void pinhole_Translate(int*** world, int height, int width, int** image_out)
{
	double ratio = FOCAL / (double)Z;

	for (int y = 0; y < height; y++) 
		for (int x = 0; x < width; x++)
		{
			int px = (int)(-x / ratio) + 700;
			int py = (int)(-y / ratio) + 600;

			int _Px = (int)(x / ratio);
			int _Py = (int)(y / ratio);

			double fx1 = (double)x / (double)ratio - _Px;
			double fx2 = (1 - fx1);
			double fy1 = (double)y / (double)ratio - _Py;
			double fy2 = (1 - fy1);

			double w1 = (fx2 * fy2);
			double w2 = (fx1 * fy2);
			double w3 = (fx2 * fy1);
			double w4 = (fx1 * fy1);

			uchar P1, P2, P3, P4;
						
			if (px > 0 && px < width - 2 && py < height - 2 && py > 0) {
				P1 = world[Z][py][px];
				P2 = world[Z][py][px + 1];
				P3 = world[Z][py + 1][px];
				P4 = world[Z][py + 1][px + 1];
			}
			else {
				P1 = 0;	P2 = 0;	P3 = 0;	P4 = 0;
			}

			if (x < 0 || x >= width - 1 || y < 0 || y >= height - 1)
				image_out[y][x] = 0;
			else
				image_out[y][x] = (w1*P1 + w2*P2 + w3*P3 + w4*P4);

			if (image_out[y][x] > 255) image_out[y][x] = 255;
		}
}

	
void main()
{
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height); //height=1024, width = 768
	int** image_out = (int**)IntAlloc2(width, height);// image_out[height][width]
	int*** img = (int***)IntAlloc3(width, height, Z);

	CopyImage3(image, height, width, img);
	pinhole_Translate(img, height, width, image_out);

	WriteImage("image_out.jpg", image_out, width, height);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width, height);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width, height);
	IntFree3(img, width, height, Z);
}