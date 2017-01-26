#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

//#include "header.h"

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

#define FOCAL_LENGTH 0.8
#define WORLD_LENGTH 1.5

using namespace cv;
using namespace std;

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

/*void rotateImage(const Mat &input, Mat &output, double alpha, double beta, double gamma, double dx, double dy, double dz, double f)
{
	alpha = (alpha - 90.)*CV_PI / 180.;
	beta = (beta - 90.)*CV_PI / 180.;
	gamma = (gamma - 90.)*CV_PI / 180.;

	// get width and height for ease of use in matrices
	double w = (double)input.cols;
	double h = (double)input.rows;

	// Projection 2D -> 3D matrix
	Mat A1 = (Mat_<double>(4, 3) <<
		1, 0, -w / 2,
		0, 1, -h / 2,
		0, 0, 0,
		0, 0, 1);

	// Rotation matrices around the X, Y, and Z axis
	Mat RX = (Mat_<double>(4, 4) <<
		1, 0, 0, 0,
		0, cos(alpha), -sin(alpha), 0,
		0, sin(alpha), cos(alpha), 0,
		0, 0, 0, 1);

	Mat RY = (Mat_<double>(4, 4) <<
		cos(beta), 0, -sin(beta), 0,
		0, 1, 0, 0,
		sin(beta), 0, cos(beta), 0,
		0, 0, 0, 1);

	Mat RZ = (Mat_<double>(4, 4) <<
		cos(gamma), -sin(gamma), 0, 0,
		sin(gamma), cos(gamma), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	// Composed rotation matrix with (RX, RY, RZ)
	Mat R = RX * RY * RZ;

	// Translation matrix
	
	Mat T = (Mat_<double>(4, 4) <<
		1, 0, 0, dx,
		0, 1, 0, dy,
		0, 0, 1, dz,
		0, 0, 0, 1);

	// 3D -> 2D matrix
	Mat A2 = (Mat_<double>(3, 4) <<
		f, 0, w / 2, 0,
		0, f, h / 2, 0,
		0, 0, 1, 0);

	// Final transformation matrix
	Mat trans = A2 * (T * (R * A1));

	// Apply matrix transformation
	warpPerspective(input, output, trans, input.size(), INTER_LANCZOS4);
}
int computePixelCoordinates(int image_plane_x, int image_plane_y, int** image, int width, int height, double pScreen_x, double pScreen_y)
{
	int _Px = (int)pScreen_x;
	int _Py = (int)pScreen_y;

	uchar result;

	if (pScreen_x < 0 || pScreen_x >= image_plane_x - 1 || pScreen_y < 0 || pScreen_y >= image_plane_y - 1)
		result = 0;
	else
		result = image[_Py][_Px];

	return (int)result;
}
void inputPixel(int image_plane_x, int image_plane_y, cv::Mat image, int width, int height, cv::Mat image_out)
{
	Point3f image_corners[4] = {
		{ 255, 512, -0.5 },
		{ 255 + (double)width, 512, -0.5 },
		{ 255 + (double)width, 512 + (double)height, -0.5 },
		{ 255, 512 + (double)height, -0.5 }
	};

	for (int i = 0; i < 4; i++) {
	Vec2f pScreen;

	pScreen.x = image_corners[i].x / -image_corners[i].z * NEAR;
	pScreen.y = image_corners[i].y / -image_corners[i].z * NEAR;
	}
	Mat A2 = (Mat_<double>(3, 3) <<
		NEAR / -image_corners->z, 0, 0,
		0, NEAR / -image_corners->z, 0,
		0, 0, 1);

	warpPerspective(image, image_out, A2, image_out.size(), INTER_LANCZOS4);

}*/

//cx=width/2, cy=height/2
void matching(double x, double y, double z, double pan_deg, double tilt_deg)
{
	// camera pose
	// word coordinate system: ground is xy plane, vertical up is z (right-handed system)
	// camera coordinte system: frontal is z, up is y, left is x axis (right-handed system)
	double m_x, m_y, m_z;	// right-handed coordinate system (xy-ground plane, z-vertical up)
	double m_pan;			// zero when frontal is parallel with world Y axis. left is +, right is - (unit: radian)
	double m_tilt;			// zero when parallel with ground plane. up is +, down is - (unit: radian)

							// coordinate transformation matrix from world to camera: Xc = m_R*(Xw - m_T)
	double m_R[3][3];		// rotation
	double m_T[3];			// translation

	m_x = x;	
	m_y = y;	
	m_z = z;
	m_pan = pan_deg*CV_PI / 180;
	m_tilt = tilt_deg*CV_PI / 180;

	m_T[0] = m_x;	
	m_T[1] = m_y;	
	m_T[2] = m_z;

	m_R[0][0] = cos(m_pan);
	m_R[0][1] = sin(m_pan);
	m_R[0][2] = 0;
	m_R[1][0] = -sin(m_tilt)*sin(m_pan);
	m_R[1][1] = sin(m_tilt)*cos(m_pan);
	m_R[1][2] = -cos(m_tilt);
	m_R[2][0] = -cos(m_tilt)*sin(m_pan);
	m_R[2][1] = cos(m_tilt)*cos(m_pan);
	m_R[2][2] = sin(m_tilt);

	//matching pairs
	vector<Point3d> w_pts1[4] = { {200.0, 200.0, 10.0} };	//3d world coordinates
	vector<Point2d> imagePoints;	//2d image coordinates

	double Xc, Yc, Zc;	// camera coornidate
	double u, v;		// normalized image coordinate
	double r2;		// undistorted radial distance
	double u_d, v_d;	// distorted normalized image coordinate
	double x, y;		// distorted pixel coordinate

	for (size_t i = 0; i < w_pts1.size(); i++)
	{
		//camera coordinate
		Xc = m_R[0][0] * (w_pts1[i].x - m_T[0]) + m_R[0][1] * (w_pts1[i].y - m_T[1]) + m_R[0][2] * (w_pts1[i].z - m_T[2]);
		Yc = m_R[1][0] * (w_pts1[i].x - m_T[0]) + m_R[1][1] * (w_pts1[i].y - m_T[1]) + m_R[1][2] * (w_pts1[i].z - m_T[2]);
		Zc = m_R[2][0] * (w_pts1[i].x - m_T[0]) + m_R[2][1] * (w_pts1[i].y - m_T[1]) + m_R[2][2] * (w_pts1[i].z - m_T[2]);
	}
	//내부 파라미터
	double fx = FOCAL_LENGTH / -worldPoints.x, fy = FOCAL_LENGTH / -worldPoints->z;


	double m[] = { fx,0,cx,
					0,fy,cy,
					0,0,1 };
	Mat cameraMatrix(3, 3, CV_64FC1, m);

	vector<Mat> rvecs;
	vector<Mat> tvecs;
	vector<Mat> distCoeffs;

	calibrateCamera(worldPoints, imagePoints, image.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

}
void main_pinhole()
{
	int image_plane_x = 1024, image_plane_y = 500;
	Mat image = imread("Hydrangeas.jpg", 1);
	Mat image_out;
	int width = image.cols;
	int height = image.rows;

	inputPixel(image_plane_x, image_plane_y, image, width, height, image_out);

	imshow("input window", image);
	imshow("output window", image_out);
	waitKey(0);

}