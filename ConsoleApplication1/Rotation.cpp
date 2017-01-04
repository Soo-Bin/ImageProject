#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;
#define PI 3.14159265359

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

void Rotation(int** image, int width, int height, int** image_out, double degree)
{
	int orig_x, orig_y;
	int pixel;
	double radian = degree*PI / 180.0;
	double cc = cos(radian), ss = sin(-radian);
	double xcenter = (double)width / 2.0, ycenter = (double)height / 2.0;

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {

			orig_x = (int)(xcenter + ((double)y - ycenter)*ss + ((double)x - xcenter)*cc);
			orig_y = (int)(ycenter + ((double)y - ycenter)*cc - ((double)x - xcenter)*ss);
			pixel = 0;

			if ((orig_y >= 0 && orig_y < height) && (orig_x >= 0 && orig_x < width)) 
				pixel = image[orig_y][orig_x];

			image_out[y][x] = pixel;
		}
}

void main_rotaion()
{
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height);
	int** image_out = (int**)IntAlloc2(width, height); // image_out[height][width]

	Rotation(image, width, height, image_out, 45);

	WriteImage("image_out.jpg", image_out, width, height);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width, height);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width, height);

}
