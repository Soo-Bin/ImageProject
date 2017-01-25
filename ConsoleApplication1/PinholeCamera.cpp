#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "header.h"

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

#define NEAR 0.1
#define Z 2.7

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

const Vec3f pWorld[6] = {
	{ 0.832343,  0.022582,  -0.018577 },
	{ 1.6878, 0.017131,  -0.022032 }
};

void computePixelCoordinates(int width, int height)
{
	int result;
	for (uint32_t i = 0; i < 2; i++) {
		Vec2f pScreen;
		pScreen.x = pWorld[i].x / -pWorld[i].z * NEAR;
		pScreen.y = pWorld[i].y / -pWorld[i].z * NEAR;

		if (pScreen.x < 0 || pScreen.x >= width - 1 || pScreen.y < 0 || pScreen.y >= height - 1)
		{
			printf("\t======FAIL======\n");
			exit(0);
		}
		printf("%f \t %f \n", pScreen.x, pScreen.y);
	}
}

/*
void main_interpolation()
{
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height);
	int** image_out = (int**)IntAlloc2(width*SIZE, height*SIZE);// image_out[height][width]

	Interpolation(image, width, height, image_out);

	WriteImage("image_out.jpg", image_out, width*SIZE, height*SIZE);

	ImageShow("input window", image, width, height);
	ImageShow("output window", image_out, width*SIZE, height*SIZE);
	waitKey(0);

	IntFree2(image, width, height);
	IntFree2(image_out, width*SIZE, height*SIZE);

}
*/
void main_pinhole() 
{
	int image_plane_x = 1024, image_plane_y = 512;
	int width, height;
	int** image = ReadImage("Hydrangeas.jpg", &width, &height);
	
	Vec3f image_point[1] = {
		{ 512, 512, 3 }
	};
	Vec3f image_out;
	for (int y = 0; y < height; y++) 
		for (int x = 0; x < width; x++) {
			image_out.x = image[y][x];
			image_out.y = image[y][x];
			image_out.z = image_point->z;
			printf("%d %d %d \n", image_out.x, image_out.y, image_out.z);
		}
	computePixelCoordinates(image_plane_x, image_plane_y);
	
	IntFree2(image, width, height);

}