#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

#define SIZE 1.3

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

int Bilinear_Interp(int** image, int height, int width, double y, double x)
{

	int _Px = (int)x;
	int _Py = (int)y;

	double delta_x = x - _Px;
	double delta_y = y - _Py;
	double result;
	
	uchar P1 = image[_Py][_Px];
	uchar P2 = image[_Py][_Px + 1];
	uchar P3 = image[_Py + 1][_Px];
	uchar P4 = image[_Py + 1][_Px + 1];

	if (x < 0 || x >= SIZE*width - 1 || y < 0 || y >= SIZE*height - 1)
		result = 0;
	else 
		result = P1*(1 - delta_x)*(1 - delta_y) + P2*(delta_x)*(1 - delta_y) + P3*(1 - delta_x)*(delta_y) + P4*(delta_x)*(delta_y);
	
	return (int)result;
}

void Interpolation(int** image, int width, int height, int** image_out)
{

	for (int y = 0; y < (int)((double)height*SIZE); y++)
		for (int x = 0; x < (int)((double)width*SIZE); x++) {

			image_out[y][x] = Bilinear_Interp(image, height, width, (double)y / SIZE, (double)x / SIZE);
			if (image_out[y][x] > 255) image_out[y][x] = 255;

		}
}

void main_interpolation()
{
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height);
	int** image_out = (int**) IntAlloc2(width*SIZE, height*SIZE);// image_out[height][width]

	Interpolation(image, width, height, image_out);

	WriteImage("image_out.jpg", image_out, width*SIZE, height*SIZE);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width*SIZE, height*SIZE);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width*SIZE, height*SIZE);

}
