//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/Button.h>
#include <gui/GridComposer.h>
#include <gui/NumericEdit.h>
#include <gui/FileDialog.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <filesystem>

class ViewCalib : public gui::View
{
private:
protected:
    gui::GridLayout _gl;
    gui::VerticalLayout _vl;
    gui::HorizontalLayout _hlbtn;
    gui::HorizontalLayout _hldist;
    gui::Label _autolbl;
    gui::Label _poljelbl;
    gui::Label _plocalbl;
    gui::Label _putlbl;

    gui::LineEdit _put;
    gui::NumericEdit _poljex;
    gui::NumericEdit _poljey;
    gui::NumericEdit _plocax;
    gui::NumericEdit _plocay;
    gui::GridComposer gc;

    gui::Button _browse;
    gui::Button _auto;
    gui::Button _manual;

    gui::Label _manuallbl;
    gui::Label _fxlbl;
    gui::Label _fylbl;
    gui::Label _cxlbl;
    gui::Label _cylbl;
    gui::Label _distlbl;
    gui::Label _alphalbl;

    gui::NumericEdit _fx;
    gui::NumericEdit _fy;
    gui::NumericEdit _cx;
    gui::NumericEdit _cy;
    gui::NumericEdit _alpha;
    gui::NumericEdit _dist1;
    gui::NumericEdit _dist2;
    gui::NumericEdit _dist3;
    gui::NumericEdit _dist4;
    gui::NumericEdit _dist5;

    cv::Mat cameraMatrix;
    cv::Mat dist;
    double angle;
public:
    ViewCalib()
    : _gl(14, 2)
    , _vl(4)
    , _autolbl("Automatska Kalibracija:")
    , _poljelbl("Dimenzije Polja:")
    , _plocalbl("Broj Polja:")
    , _putlbl("Put do slika:")
    , _poljex(td::int4)
    , _poljey(td::int4)
    , _plocax(td::int4)
    , _plocay(td::int4)
    , gc(_gl)
    , _browse("Pretraži")
    , _auto("Auto")
    , _manual("Manual")
    , _manuallbl("Manualna Kalibracija:")
    , _fxlbl("Fx=")
    , _cxlbl("Cx=")
    , _fylbl("Fy=")
    , _cylbl("Cy=")
    , _alphalbl("alpha=")
    , _fx(td::real8)
    , _fy(td::real8)
    , _cx(td::real8)
    , _cy(td::real8)
    , _alpha(td::real8)
    , _hlbtn(3)
    , _hldist(6)
    , _dist1(td::real8)
    , _dist2(td::real8)
    , _dist3(td::real8)
    , _dist4(td::real8)
    , _dist5(td::real8)
    , _distlbl("Distorzijski koeficijenti:")
    {
        _browse.setType(gui::Button::Type::Default);

        gc.appendRow(_autolbl, 2, td::HAlignment::Center);
        gc.appendRow(_putlbl);
        gc.appendRow(_put);
        gc.appendCol(_browse);
        gc.appendRow(_poljelbl);
        gc.appendRow(_poljex);
        gc.appendCol(_poljey);
        gc.appendRow(_plocalbl);
        gc.appendRow(_plocax);
        gc.appendCol(_plocay);
        gc.appendRow(_manuallbl, 2, td::HAlignment::Center);
        gc.appendRow(_fxlbl, td::HAlignment::Right);
        gc.appendCol(_fx);
        gc.appendRow(_fylbl, td::HAlignment::Right);
        gc.appendCol(_fy);
        gc.appendRow(_cxlbl, td::HAlignment::Right);
        gc.appendCol(_cx);
        gc.appendRow(_cylbl, td::HAlignment::Right);
        gc.appendCol(_cy);
        gc.appendRow(_alphalbl, td::HAlignment::Right);
        gc.appendCol(_alpha);

        _hlbtn.appendSpacer();
        _hlbtn.append(_auto);
        _hlbtn.append(_manual);

        _hldist.append(_distlbl);
        _hldist.append(_dist1);
        _hldist.append(_dist2);
        _hldist.append(_dist3);
        _hldist.append(_dist4);
        _hldist.append(_dist5);

        _vl.append(_gl);
        _vl.append(_hldist);
        _vl.append(_hlbtn);
        _vl.appendSpacer();
        setLayout(&_vl);

        //cv::Mat image;
        //image = cv::imread("apple-1.bmp", cv::IMREAD_COLOR_RGB);
        //cv::imshow("prozor", image);
        //cv::waitKey(0);
    }
    bool onClick(gui::Button* pBtn) override {
        if (pBtn == &_browse) {
            showSelectFolderDialog();
        }
        if (pBtn == &_auto) {
            kalibracija();
        }
        if (pBtn == &_manual) {
            double fx = _fx.getValue().r8Val();
            double fy = _fy.getValue().r8Val();
            double cx = _cx.getValue().r8Val();
            double cy = _cy.getValue().r8Val();
            cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
            cameraMatrix.at<double>(0, 0) = fx;
            cameraMatrix.at<double>(1, 1) = fy;
            cameraMatrix.at<double>(0, 2) = cx;
            cameraMatrix.at<double>(1, 2) = cy;

            double dist1=_dist1.getValue().r8Val();
            double dist2=_dist2.getValue().r8Val();
            double dist3=_dist3.getValue().r8Val();
            double dist4=_dist4.getValue().r8Val();
            double dist5=_dist5.getValue().r8Val();

            dist.at<double>(0, 0)=dist1;
            dist.at<double>(1, 0)=dist2;
            dist.at<double>(2, 0)=dist3;
            dist.at<double>(3, 0)=dist4;
            dist.at<double>(4, 0)=dist5;

            angle = _alpha.getValue().r8Val();
        }
        return false;
    }

    void showSelectFolderDialog() {
        gui::SelectFolderDialog::show(this->getParentWindow(), "Kalibracija", 2);
    }

    void setPath(td::String str) {
        _put.setValue(str);
    }

    std::vector<cv::Point2f> getCorners(std::string path, cv::Mat& slika) {
        int HEI=_plocax.getValue().i4Val();
        int WID=_plocay.getValue().i4Val();
        std::vector<cv::Point2f> corners;
        slika = cv::imread(path, cv::IMREAD_COLOR);
        cv::Mat gray;
        cv::cvtColor(slika, gray, cv::COLOR_BGR2GRAY);
        cv::findChessboardCorners(gray, cv::Size(HEI, WID), corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);
        cv::cornerSubPix(gray, corners, cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 3000, 0.000001));
        cv::drawChessboardCorners(slika, cv::Size(HEI, WID), corners, true);
        cv::imshow("slika", slika);
        cv::waitKey(0);
        return corners;
    }
    
    void kalibracija() {
        td::Variant var;
        _put.getValue(var);
        td::String path = var.strVal();
        std::vector<cv::Mat> slike;
        std::vector<std::vector<cv::Point2f> >corners;
        int i = 0;
        for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
            std::string s = entry.path().string();
            slike.push_back(cv::Mat());
            corners.push_back(getCorners(s, slike[i]));
            i++;
        }
        int HEI = _plocax.getValue().i4Val();
        int WID = _plocay.getValue().i4Val();
        int HEI1 = _poljex.getValue().i4Val();
        int WID1 = _poljey.getValue().i4Val();
        std::vector<cv::Point3f> obj;
        for (int i = 0; i < WID; i++) {
            for (int j = 0; j < HEI; j++) {
                obj.push_back(cv::Point3f(i * 1.0f * WID1, j * 1.0f * HEI1, 0));
            }
        }
        int BR_SLIKA = slike.size();
        std::vector<cv::Mat> rvecs, tvecs;
        dist = cv::Mat::zeros(5, 1, CV_64F);

        std::vector<std::vector<cv::Point3f> >objects(BR_SLIKA, obj);

        double error = cv::calibrateCamera(objects, corners, slike[0].size(), cameraMatrix, dist, rvecs, tvecs);
        for (int i = 0; i < BR_SLIKA; i++) {
            cv::solvePnP(objects[i], corners[i], cameraMatrix, dist, rvecs[i], tvecs[i], false, cv::SOLVEPNP_ITERATIVE);
        }

        _fx.setValue(cameraMatrix.at<double>(0, 0));
        _fy.setValue(cameraMatrix.at<double>(1, 1));
        _cx.setValue(cameraMatrix.at<double>(0, 2));
        _cy.setValue(cameraMatrix.at<double>(1, 2));

        _dist1.setValue(dist.at<double>(0, 0));
        _dist2.setValue(dist.at<double>(1, 0));
        _dist3.setValue(dist.at<double>(2, 0));
        _dist4.setValue(dist.at<double>(3, 0));
        _dist5.setValue(dist.at<double>(4, 0));

        cv::Mat R;
        cv::Rodrigues(rvecs[0], R);
        angle = 180.0 / CV_PI * std::asin(-R.at<double>(2, 0));
        _alpha.setValue(angle);
    }
};
