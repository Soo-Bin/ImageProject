#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

void main_CircleImage();
void main_AvgFiltering();
void main_interpolation();
void main_rotaion();
int main_LifeCam();
void main_pinhole();

void main()
{
	main_CircleImage();
	main_AvgFiltering();
	main_interpolation();
	main_rotaion();
	main_LifeCam();
    main_pinhole();

}
