#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include <opencv2/opencv.hpp>   
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;
using namespace std;

int** IntAlloc2(int width, int height);
void IntFree2(int** image, int width, int height);
int** ReadImage(char* name, int* width, int* height);
void WriteImage(char* name, int** image, int width, int height);
void ImageShow(char* winname, int** image, int width, int height);
void CopyImage(int** src, int width, int height, int** dst);

// camera intrinsic parameters
double m_fx, m_fy;
double m_cx, m_cy;
double m_k1, m_k2;
double m_p1, m_p2;

// valid range of u,v (normalized undistorted coordinates)
int m_img_w;
int m_img_h;
double m_min_u, m_max_u;
double m_min_v, m_max_v;

// camera pose
// word coordinate system: ground is xy plane, vertical up is z (right-handed system)
// camera coordinte system: frontal is z, up is y, left is x axis (right-handed system)
double m_x, m_y, m_z;	// right-handed coordinate system (xy-ground plane, z-vertical up)
double m_pan;			// zero when frontal is parallel with world Y axis. left is +, right is - (unit: radian)
double m_tilt;			// zero when parallel with ground plane. up is +, down is - (unit: radian)

						// coordinate transformation matrix from world to camera: Xc = m_R*(Xw - m_T)
double m_R[3][3];		// rotation
double m_T[3];			// translation

void cam_setting(double x, double y, double z, double pan_deg, double tilt_deg)
{
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
}
void cam_parameter(double fx, double fy, double cx, double cy, double k1, double k2, double p1, double p2)
{
	m_fx = fx;
	m_fy = fy;
	m_cx = cx;
	m_cy = cy;
	m_k1 = k1;
	m_k2 = k2;
	m_p1 = p1;
	m_p2 = p2;
}
void set_img(int img_w, int img_h)
{
	m_img_w = img_w;
	m_img_h = img_h;

	double x, y, u, v;

	x = 0;
	y = 0;
	u = (x - m_cx) / m_fx;
	v = (y - m_cy) / m_fy;
	//undistort_normal(u, v);

	m_min_u = u;
	m_min_v = v;

	x = img_w;
	y = img_h;
	u = (x - m_cx) / m_fx;
	v = (y - m_cy) / m_fy;
	//undistort_normal(u, v);

	m_max_u = u;
	m_max_v = v;
}
void pin_draw(const vector<Point3d>& w_pts1, vector<Point2d>& i_pts2, vector<double>& depth)
{
	double Xc, Yc, Zc;	// camera coornidate
	double u, v;		// normalized image coordinate
	double r2;		// undistorted radial distance
	double u_d, v_d;	// distorted normalized image coordinate
	double x, y;		// distorted pixel coordinate
	
	i_pts2.clear();
	depth.clear();

	for (size_t i = 0; i < w_pts1.size(); i++)
	{
		//camera coordinate
		Xc = m_R[0][0] * (w_pts1[i].x - m_T[0]) + m_R[0][1] * (w_pts1[i].y - m_T[1]) + m_R[0][2] * (w_pts1[i].z - m_T[2]);
		Yc = m_R[1][0] * (w_pts1[i].x - m_T[0]) + m_R[1][1] * (w_pts1[i].y - m_T[1]) + m_R[1][2] * (w_pts1[i].z - m_T[2]);
		Zc = m_R[2][0] * (w_pts1[i].x - m_T[0]) + m_R[2][1] * (w_pts1[i].y - m_T[1]) + m_R[2][2] * (w_pts1[i].z - m_T[2]);

		if (Zc == 0)
		{
			i_pts2.push_back(Point2d(0, 0));
			depth.push_back(0);
			continue;
		}

		// normalized image coordinate
		u = -Xc / Zc;
		v = -Yc / Zc;

		// distorted normalized image coordinate
		r2 = u*u + v*v;
		u_d = (1 + m_k1*r2 + m_k2*r2*r2)*u + 2 * m_p1*u*v + m_p2*(r2 + 2 * u*u);
		v_d = (1 + m_k1*r2 + m_k2*r2*r2)*v + m_p1*(r2 + 2 * v*v) + 2 * m_p2*u*v;

		// pixel coordinate
		x = -u_d*m_fx + m_cx;
		y = -v_d*m_fy + m_cy;

		i_pts2.push_back(Point2d(x, y));
		depth.push_back(Zc);
	}
	printf("3D: %f %f %f \t 2D: %f %f \n", Xc, Yc, Zc, u, v);

}

void main_pinhole()
{
	//camera position
	double cp_x = 0.5, cp_y = 0, cp_z = 1.5;
	double pan_deg = 0, tilt_deg = 0;

	//camera parameters
	double fx = 300, fy = 300, cx = 320, cy = 240;
	double k1 = 0, k2 = 0, p1 = 0, p2 = 0;
	int img_w = 640, img_h = 480;

	//coordinate
	vector<Point3d> worldPoint = { { 1.3, 1.3, 0.8 },
									{ 1.8, 1.3, 0.8 },
									{ 1.8, 1.8, 0.8 },
									{ 1.3, 1.8, 0.8 }
	};
	vector<Point2d> imagePoint;
	vector<double> depth = { 3.0 };
	//Mat image = imread("Hydrangeas.jpg", 1);
	//Mat image_out;
	//cvtColor(image, image, CV_GRAY2RGB);

	cam_setting(cp_x, cp_x, cp_z, pan_deg, tilt_deg);
	cam_parameter(fx, fy, cx, cy, k1, k2, p1, p2);
	set_img(img_w, img_h);
	pin_draw(worldPoint, imagePoint, depth);

	//imshow("input window", image);
	//imshow("output window", image_out);
	//waitKey(0);

}