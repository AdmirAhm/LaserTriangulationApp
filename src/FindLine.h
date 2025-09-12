#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

bool Step1(cv::Mat& pSrc, cv::Mat& pDst) {
	bool t = false;
	pSrc.copyTo(pDst);
	for (int i = 1; i < pSrc.rows - 1; i++) {
		for (int j = 1; j < pSrc.cols - 1; j++) {
			if (pSrc.at<uchar>(i, j) > 0) {
				int n1 = (int)pSrc.at<uchar>(i, j);
				int n2 = (int)pSrc.at<uchar>(i - 1, j) / 255;
				int n3 = (int)pSrc.at<uchar>(i - 1, j + 1) / 255;
				int n4 = (int)pSrc.at<uchar>(i, j + 1) / 255;
				int n5 = (int)pSrc.at<uchar>(i + 1, j + 1) / 255;
				int n6 = (int)pSrc.at<uchar>(i + 1, j) / 255;
				int n7 = (int)pSrc.at<uchar>(i + 1, j - 1) / 255;
				int n8 = (int)pSrc.at<uchar>(i, j - 1) / 255;
				int n9 = (int)pSrc.at<uchar>(i - 1, j - 1) / 255;

				int A = (n2 < n3) +
					(n3 < n4) +
					(n4 < n5) +
					(n5 < n6) +
					(n6 < n7) +
					(n7 < n8) +
					(n8 < n9) +
					(n9 < n2);
				int B = (n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9);
				if (A == 1 && B >= 2 && B <= 6 && (n2 < 1 || n4 < 1 || n6 < 1) && (n8 < 1 || n4 < 1 || n6 < 1)) {
					pDst.at<uchar>(i, j) = 0;
					t = true;
				}

			}
		}
	}
	return t;
}

bool Step2(cv::Mat& pSrc, cv::Mat& pDst) {
	bool t = false;
	pSrc.copyTo(pDst);
	for (int i = 1; i < pSrc.rows - 1; i++) {
		for (int j = 1; j < pSrc.cols - 1; j++) {
			if (pSrc.at<uchar>(i, j) > 0) {
				int n1 = (int)pSrc.at<uchar>(i, j);
				int n2 = (int)pSrc.at<uchar>(i - 1, j) / 255;
				int n3 = (int)pSrc.at<uchar>(i - 1, j + 1) / 255;
				int n4 = (int)pSrc.at<uchar>(i, j + 1) / 255;
				int n5 = (int)pSrc.at<uchar>(i + 1, j + 1) / 255;
				int n6 = (int)pSrc.at<uchar>(i + 1, j) / 255;
				int n7 = (int)pSrc.at<uchar>(i + 1, j - 1) / 255;
				int n8 = (int)pSrc.at<uchar>(i, j - 1) / 255;
				int n9 = (int)pSrc.at<uchar>(i - 1, j - 1) / 255;

				int A = (n2 < n3) +
					(n3 < n4) +
					(n4 < n5) +
					(n5 < n6) +
					(n6 < n7) +
					(n7 < n8) +
					(n8 < n9) +
					(n9 < n2);
				int B = (n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9);
				if (A == 1 && B >= 2 && B <= 6 && (n2 < 1 || n4 < 1 || n8 < 1) && (n8 < 1 || n2 < 1 || n6 < 1)) {
					pDst.at<uchar>(i, j) = 0;
					t = true;
				}

			}
		}
	}
	return t;
}

void ZhangSuen(cv::Mat& pSrc, cv::Mat& pDst) {
	while (1) {
		bool t1 = Step1(pSrc, pDst);
		pDst.copyTo(pSrc);
		bool t2 = Step2(pSrc, pDst);
		pDst.copyTo(pSrc);
		if (!t1 && !t2) {
			return;
		}
	}
}

void findLine(std::string path) {
	int h = 49, s = 88, v = 28, H = 68, S = 255, V = 255;
	h = 35, s = 36, v = 210, H = 80, S = 219, V = 255;
	int i = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
		std::string pth = entry.path().string();
		std::string nm = entry.path().filename().string();
		cv::Mat slika, slikaMod, green, green2;
		slika = cv::imread(pth, cv::IMREAD_COLOR);
		cv::cvtColor(slika, slikaMod, cv::COLOR_BGR2HSV);
		if (i == 0) {
			//cv::GaussianBlur(slikaMod, slikaMod, cv::Size(3, 3), 0.3, 0.3);
			cv::namedWindow("tb");

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
			cv::destroyWindow("win");
			cv::destroyWindow("tb");
		}
		else {
			cv::inRange(slikaMod, cv::Scalar(h, s, v), cv::Scalar(H, S, V), green);
		}
		i++;
		for (int i = 0; i < 20; i++) {
			ZhangSuen(green, green2);
			green2.copyTo(green);
		}
		std::stringstream ss;
		ss << path << "/m" << nm;
		std::string imeslike;
		ss >> imeslike;
		cv::imwrite(imeslike, green);

	}
}