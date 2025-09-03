#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <ueye.h>

void camera() {
	HIDS hcam = 0;
	char* pMem = nullptr;
	int memId = 0;

	if (is_InitCamera(&hcam, nullptr) != IS_SUCCESS) {
		return;
	}

	is_SetColorMode(hcam, IS_CM_BGR8_PACKED);

	SENSORINFO sensorInfo;
	is_GetSensorInfo(hcam, &sensorInfo);
	std::cout << "Sensor ID: " << sensorInfo.SensorID << std::endl;
	//std::cout << "Sensor Type: " << sensorInfo.SensorType << std::endl;
	std::cout << "Sensor Name: " << sensorInfo.strSensorName << std::endl;
	std::cout << "Master Gain: " << (int)sensorInfo.bMasterGain << std::endl;
	std::cout << "RGGB Gain: " << (int)sensorInfo.bRGain << std::endl;
	std::cout << "Global Shutter: " << (int)sensorInfo.bGlobShutter << std::endl;

	int width = 1280, height = 720;
	IS_RECT rect = { 0, 0, width, height };
	is_AOI(hcam, IS_AOI_IMAGE_SET_AOI, &rect, sizeof(rect));

	if (is_AllocImageMem(hcam, width, height, 24, &pMem, &memId) != IS_SUCCESS) {
		return;
	}
	is_SetImageMem(hcam, pMem, memId);
	is_SetDisplayMode(hcam, IS_SET_DM_DIB);
	is_CaptureVideo(hcam, IS_DONT_WAIT);

	cv::Mat frame(height, width, CV_8UC3);
	int fmin, fmax, finc;
	double min;
	int i = 0;
	double pval1 = 0, pval2 = 0;
	int focus = 1;

	is_SetAutoParameter(hcam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &pval1, &pval2);
	is_SetAutoParameter(hcam, IS_SET_ENABLE_AUTO_GAIN, &pval1, &pval2);
	is_SetAutoParameter(hcam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &pval1, &pval2);
	is_SetAutoParameter(hcam, IS_SET_ENABLE_AUTO_SHUTTER, &pval1, &pval2);
	is_SetGainBoost(hcam, IS_SET_GAINBOOST_OFF);

	while (true) {
		is_Focus(hcam, FOC_CMD_SET_MANUAL_FOCUS, &focus, 0);
		is_FreezeVideo(hcam, IS_WAIT);
		memcpy(frame.data, pMem, width * height * 3);
		cv::imshow("uEye Camera", frame);
		auto chr = cv::waitKey(1);
		if (chr == 'q') break;
		if (chr == 'a') {
			is_SetFrameRate(hcam, 2, &min);
		}
		if (chr == 'd') {
			is_SetFrameRate(hcam, 15, &min);
		}
		if (chr == 'c') {
			std::stringstream ss;
			ss << "slika";
			ss << i;
			ss << ".jpg";
			std::string pom;
			ss >> pom;
			cv::imwrite(pom, frame);
			i++;
		}
		if (chr == 'p') {
			is_Focus(hcam, FOC_CMD_SET_ENABLE_AUTOFOCUS, NULL, 0);
		}
		if (chr == 'm') {
			is_Focus(hcam, FOC_CMD_SET_DISABLE_AUTOFOCUS, NULL, 0);
		}
		if (chr == 's') {
			is_SetAutoParameter(hcam, IS_GET_ENABLE_AUTO_SENSOR_GAIN, &pval2, &pval1);
			std::cout << pval2;
		}


	}
	cv::destroyWindow("uEye Camera");
	is_StopLiveVideo(hcam, IS_FORCE_VIDEO_STOP);
	is_FreeImageMem(hcam, pMem, memId);
	is_ExitCamera(hcam);

}
