#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>

std::vector<cv::Point2f> PreparePixel(cv::Mat pixel, cv::Mat A, cv::Mat dist) {
	std::vector<cv::Point2f> fin(1);
	cv::undistortPoints(pixel, fin, A, dist);
	return fin;
}

void PixelToWorld(cv::Mat pixel, cv::Mat R, cv::Mat t, cv::Mat& dir, cv::Mat& origin) {
	cv::Mat dcam = pixel;
	cv::Mat R_inv;
	cv::transpose(R, R_inv);
	dir = R_inv * dcam;
	cv::normalize(dir, dir);
	origin = -R_inv * t;
}

cv::Mat IntersectionPoint(cv::Mat dir, cv::Mat origin, cv::Mat planepoint, cv::Mat planeort) {
	cv::Mat pom1 = (planepoint - origin);
	cv::transpose(pom1, pom1);
	pom1 = pom1 * planeort;
	cv::Mat pom2 = dir;
	cv::transpose(pom2, pom2);
	pom2 = pom2 * planeort;
	auto t = pom1.at<double>(0, 0) / pom2.at<double>(0, 0);
	cv::Mat inter = origin + t * dir;
	return inter;
}

cv::Point3f positionEstimation(int x, int y, cv::Mat cameraMatrix, cv::Mat distCoefs, cv::Mat R, cv::Mat t) {
	cv::Mat planepoint(3, 1, CV_64F);
	planepoint.at<double>(0, 0) = 232.639;
	planepoint.at<double>(1, 0) = 160.847;
	planepoint.at<double>(2, 0) = 0;

	cv::Mat ort(3, 1, CV_64F);
	ort.at<double>(1, 0) = 0.999;
	ort.at<double>(0, 0) = 0.044;
	ort.at<double>(2, 0) = 0;
	cv::normalize(ort, ort);

	cv::Mat pt(2, 1, CV_32F);
	pt.at<float>(0, 0) = x;
	pt.at<float>(1, 0) = y;
	std::vector<cv::Point2f>undistpts = PreparePixel(pt, cameraMatrix, distCoefs);

	cv::Mat pixel(3, 1, CV_64F);
	pixel.at<double>(0, 0) = undistpts[0].x;
	pixel.at<double>(1, 0) = undistpts[0].y;
	pixel.at<double>(2, 0) = 1;

	cv::Mat o, d;
	PixelToWorld(pixel, R, t, d, o);
	cv::Mat point = IntersectionPoint(d, o, planepoint, ort);

	cv::Point3f rez;
	rez.x = point.at<double>(0, 0);
	rez.y = point.at<double>(1, 0);
	rez.z = point.at<double>(2, 0);
	return rez;
}

void click_event(int event, int x, int y, int flags, void* params) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		cv::Mat* matrice = (cv::Mat*)params;
		auto rez = positionEstimation(x, y, matrice[0], matrice[1], matrice[2], matrice[3]);
		std::cout << rez.x << "," << rez.y << "," << rez.z << std::endl;
	}
}

void height(std::string path, cv::Mat cameraMatrix, cv::Mat dist, cv::Mat R, cv::Mat t) {
	cv::Mat slika = cv::imread(path);
	cv::imshow("Slika", slika);
	cv::Mat matrice[4];
	matrice[0] = cameraMatrix;
	matrice[1] = dist;
	matrice[2] = R;
	matrice[3] = t;
	cv::setMouseCallback("Slika", click_event, matrice);
	cv::waitKey(0);
}