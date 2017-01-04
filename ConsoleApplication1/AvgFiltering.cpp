#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

#define NUMBER 11

using namespace cv;

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

void AvgImage(int** src, int width, int height, int** dst)
{
	int avgNum = NUMBER / 2;
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			if ((x - avgNum) < 0 || (x + avgNum) >= width || (y - avgNum) < 0 || (y + avgNum) >= height) {
				dst[y][x] = src[y][x];
			}
			else {
				for (int i = (y - avgNum); i <= (y + avgNum); i++) {
					for (int j = (x - avgNum); j <= (x + avgNum); j++) {
						dst[y][x] += src[i][j];
					}
				}
				dst[y][x] /= NUMBER * NUMBER;
				if (dst[y][x] > 255)
					dst[y][x] = 255;
			}
		}
	}

}
void main_AvgFiltering()
{
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height);
	int** image_out = (int**)IntAlloc2(width, height); // image_out[height][width]

	CopyImage(image, width, height, image_out);
	AvgImage(image, width, height, image_out);
	
	WriteImage("image_out.jpg", image_out, width, height);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width, height);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width, height);

}
