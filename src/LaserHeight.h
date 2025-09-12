#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "Height.h"
#include <xml/Writer.h>

void LaserHeight(std::string path, cv::Mat cameraMatrix, cv::Mat dist, cv::Mat R, cv::Mat t) {
	int ind = 1;
	for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
		std::string s = entry.path().string();
		std::string nm = entry.path().stem().string();
		cv::Mat mask = cv::imread(s, cv::IMREAD_GRAYSCALE);
		//cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);
		//cv::threshold(mask, mask, 127, 255, cv::THRESH_BINARY);
		//cv::imshow("maska", mask);
		std::stringstream ss;
		ss << path << "/data_" << nm <<".xml";
		std::string pth;
		ss >> pth;
		xml::Writer w(pth);
		w.startDocument();
		w.startNode("Points");
		for (int i = 0; i < 720; i++) {
			for (int j = 0; j < 1280; j++) {
				if (mask.at<uchar>(i, j) > 170) {
					auto x = mask.at<uchar>(i, j);
					auto rez = positionEstimation(j, i, cameraMatrix, dist, R, t);
					w.startNode("Point");
					w.attribute("X", rez.x);
					w.attribute("Y", rez.y);
					w.attribute("Z", rez.z);
					w.endNode();
				}
			}
		}
		w.endDocument();
		ind++;
	}

}

