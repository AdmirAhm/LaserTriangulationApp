#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

void ThinSubiteration1(cv::Mat& pSrc, cv::Mat& pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	for (int i = 1; i < rows - 1; i++) {
		for (int j = 1; j < cols - 1; j++) {
			if (pSrc.at<uchar>(i, j) > 0) {
				/// get 8 neighbors
				/// calculate C(p)
				int n = (int)pSrc.at<uchar>(i, j);
				int neighbor0 = (int)pSrc.at<uchar>(i - 1, j - 1) / 255;
				int neighbor1 = (int)pSrc.at<uchar>(i - 1, j) / 255;
				int neighbor2 = (int)pSrc.at<uchar>(i - 1, j + 1) / 255;
				int neighbor3 = (int)pSrc.at<uchar>(i, j + 1) / 255;
				int neighbor4 = (int)pSrc.at<uchar>(i + 1, j + 1) / 255;
				int neighbor5 = (int)pSrc.at<uchar>(i + 1, j) / 255;
				int neighbor6 = (int)pSrc.at<uchar>(i + 1, j - 1) / 255;
				int neighbor7 = (int)pSrc.at<uchar>(i, j - 1) / 255;
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

void findLine(std::string path) {
	int h = 49, s = 88, v = 28, H = 68, S = 255, V = 255;
	h = 35, s = 36, v = 210, H = 80, S = 219, V = 255;
	int i = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
		std::string pth = entry.path().string();
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
			ThinSubiteration1(green, green2);
			green2.copyTo(green);
		}
		std::stringstream ss;
		ss << path << "/maska" << i << ".jpg";
		std::string imeslike;
		ss >> imeslike;
		cv::imwrite(imeslike, green);

	}
}