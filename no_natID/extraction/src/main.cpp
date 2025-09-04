#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>

cv::Mat cameraMatrix;
cv::Mat distCoefs;
cv::Mat R, t;
cv::Mat T;
void getGreenMask(cv::Mat &green) {
	cv::Mat slika, slikaMod;
	slika = cv::imread("../slike/slikaA.jpg", cv::IMREAD_COLOR);
	cv::cvtColor(slika, slikaMod, cv::COLOR_BGR2HSV);
	//cv::GaussianBlur(slikaMod, slikaMod, cv::Size(3, 3), 0.3, 0.3);
	cv::namedWindow("tb");
	int h=49, s=88, v=28, H=68, S=255, V=255;
	h=35, s=36, v=210, H=80, S=219, V=255;
	cv::createTrackbar("Hmin", "tb", &h, 255);
	cv::createTrackbar("Smin", "tb", &s, 255);
	cv::createTrackbar("Vmin", "tb", &v, 255);
	cv::createTrackbar("Hmax", "tb", &H, 255);
	cv::createTrackbar("Smax", "tb", &S, 255);
	cv::createTrackbar("Vmax", "tb", &V, 255);
	while (1) {
		cv::inRange(slikaMod, cv::Scalar(h, s, v), cv::Scalar(H, S, V), green);
		cv::imshow("win", green);
		if (cv::waitKey(1) == 'q') break;
	}
	//cv::destroyWindow("win");
	cv::destroyWindow("tb");
}

void kalibracija() {
	cv::Mat slika;
	std::vector<std::vector<cv::Point2f> >corners(6);
	for (int i = 0; i < 6; i++) {
		std::stringstream ss;
		ss << "../slike/slika";
		ss << i;
		ss << ".jpg";
		std::string sl;
		ss >> sl;
		slika = cv::imread(sl, cv::IMREAD_COLOR);
		cv::Mat gray;
		cv::cvtColor(slika, gray, cv::COLOR_BGR2GRAY);
		auto x = cv::findChessboardCorners(gray, cv::Size(5, 6), corners[i], cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);
		cv::cornerSubPix(gray, corners[i], cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 3000, 0.000001));
		cv::drawChessboardCorners(slika, cv::Size(5, 6), corners[i], true);
		//cv::imshow("slika", slika);
		//cv::waitKey(0);
	}
	
	std::vector<cv::Point3f> obj;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			obj.push_back(cv::Point3f(i * 1.0f * 25, j * 1.0f * 40, 0));
		}
	}
	std::vector<std::vector<cv::Point3f> >objects(6, obj);
	
	std::vector<cv::Mat> rvecs, tvecs;
	double error = cv::calibrateCamera(objects, corners, slika.size(), cameraMatrix, distCoefs, rvecs, tvecs);
	std::cout << error << std::endl;
	std::cout << cameraMatrix;
	cv::Rodrigues(rvecs[5], R);
	t = tvecs[5];
	//std::cout << R << std::endl;
	//std::cout << tvecs[5] << std::endl;

	T = cv::Mat::eye(4, 4, CV_64F);
	R.copyTo(T(cv::Rect(0, 0, 3, 3)));
	tvecs[5].copyTo(T(cv::Rect(3, 0, 1, 3)));
	cv::invert(T, T);
	//std::cout << T;

}
std::vector<cv::Point3f>spaseneTacke;

std::vector<cv::Point2f> PreparePixel(cv::Mat pixel, cv::Mat A, cv::Mat dist) {
	std::vector<cv::Point2f> fin(1);
	cv::undistortPoints(pixel, fin, A, dist);
	return fin;
}

void PixelToWorld(cv::Mat pixel, cv::Mat R, cv::Mat t, cv::Mat &dir, cv::Mat &origin) {
	cv::Mat dcam = pixel;
	cv::Mat R_inv;
	cv::transpose(R, R_inv);
	dir = R_inv*dcam;
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

cv::Point3f positionEstimation(int x, int y) {
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
		auto rez=positionEstimation(x, y);
		std::cout << rez.x << "," << rez.y << "," << rez.z <<std::endl;
	}
}

void savePoints(cv::Mat mask) {
	std::ofstream of("data.csv");

	for (int i = 0; i < 720; i++) {
		for (int j = 0; j < 1280; j++) {
			if (mask.at<uchar>(i, j) > 0) {
				auto rez = positionEstimation(j, i);
				spaseneTacke.push_back(rez);
				of << rez.x << "," << rez.y << "," << rez.z << "\n";
			}
		}
	}
	of.close();
}

void ThinSubiteration1(cv::Mat& pSrc, cv::Mat& pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	for (int i = 1; i < rows-1; i++) {
		for (int j = 1; j < cols-1; j++) {
			if (pSrc.at<uchar>(i, j) > 0) {
				/// get 8 neighbors
				/// calculate C(p)
				int n = (int)pSrc.at<uchar>(i, j);
				int neighbor0 = (int)pSrc.at<uchar>(i - 1, j - 1)/255;
				int neighbor1 = (int)pSrc.at<uchar>(i - 1, j)/255;
				int neighbor2 = (int)pSrc.at<uchar>(i - 1, j + 1)/255;
				int neighbor3 = (int)pSrc.at<uchar>(i, j + 1)/255;
				int neighbor4 = (int)pSrc.at<uchar>(i + 1, j + 1)/255;
				int neighbor5 = (int)pSrc.at<uchar>(i + 1, j)/255;
				int neighbor6 = (int)pSrc.at<uchar>(i + 1, j - 1)/255;
				int neighbor7 = (int)pSrc.at<uchar>(i, j - 1)/255;
				int C = int(~neighbor1 & (neighbor2 | neighbor3)) +
					int(~neighbor3 & (neighbor4 | neighbor5)) +
					int(~neighbor5 & (neighbor6 | neighbor7)) +
					int(~neighbor7 & (neighbor0 | neighbor1));
				if (C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
						int(neighbor2 | neighbor3) +
						int(neighbor4 | neighbor5) +
						int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
						int(neighbor3 | neighbor4) +
						int(neighbor5 | neighbor6) +
						int(neighbor7 | neighbor0);
					int N = std::min(N1, N2);
					if ((N == 2) || (N == 3)) {
						/// calculate criteria 3
						int c3 = (neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
						if (c3 == 0) {
							pDst.at<uchar>(i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}



int main() {
	kalibracija();
	
	cv::Mat green;
	getGreenMask(green);
	cv::Size s = green.size();
	cv::Mat green2;
	for (int i = 0; i < 20; i++) {
		ThinSubiteration1(green, green2);
		green2.copyTo(green);
	}
	savePoints(green);
	cv::namedWindow("Maska", cv::WINDOW_NORMAL);
	cv::resizeWindow("Maska", s);
	cv::imshow("Maska", green);
	cv::setMouseCallback("Maska", click_event);
	cv::waitKey(0);
	return 0;
}