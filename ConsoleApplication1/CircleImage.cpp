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

void main_CircleImage()
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

	WriteImage("image_out.jpg", image_out, width, height);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width, height);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width, height);

}
