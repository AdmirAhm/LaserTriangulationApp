//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ViewTestGL2.h"
#include "ToolBar.h";
#include "DynTabView.h"
#include "ViewCalib.h"
#include "Camera.h"
#include "Height.h"
#include "FindLine.h"
#include "LaserHeight.h"
#include <gui/FileDialog.h>
#include  "View2D.h";
class MainWindow : public gui::Window
{
private:
protected:
    ToolBar _mainToolBar;
    MenuBar _mainMenuBar;
    DynTabView _view;
    gui::Image _camera;
    gui::Image _2d;
    gui::Image _3d;
    gui::Image _scatter;
    gui::Image _target;
    gui::Image _height;
    gui::Image _laser;
    cv::Mat cameraMatrix;
    cv::Mat dist;
    cv::Mat r, t;

public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    , _camera(":camera")
    , _2d(":2d")
    , _laser(":laser")
    , _3d(":3d")
    , _scatter(":scatter")
    , _target(":target")
    , _height(":height")
    {
        cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        dist = cv::Mat::zeros(5, 1, CV_64F);

        //_mainToolBar.showLabels(false);
        _mainToolBar.setIconSize(gui::ToolBar::IconSize::Regular);
        setToolBar(_mainToolBar);
        _mainMenuBar.setAsMain(this);
        setCentralView(&_view);
    }
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override{
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        if (menuID == 20 && firstSubMenuID == 0 && lastSubMenuID == 0) {
            ViewCalib* pv;
            switch (actionID) {
            case 10:
                pv = new ViewCalib(&cameraMatrix, &dist, &r, &t);
                _view.addView(pv, tr("Calibration"), &_target);
                break;
            case 20:
                camera();
                break;
            case 30:
                gui::OpenFileDialog::show(this, "Height", { {tr("Slika"),"*.jpg"} }, 30);
                break;
            case 40:
                gui::SelectFolderDialog::show(this, "FindLine", 40);
                break;
            case 50:
                gui::SelectFolderDialog::show(this, "LaserHeight", 50);
                break;
            case 60:
                gui::OpenFileDialog::show(this, "2D", { {tr("XML file"),"*.xml"} }, 60);
                //pv2d = new View2D;
                //_view.addView(pv2d, tr("2D"), &_2d);
                break;
            }
            return true;
        }
        return false;
    }

    std::vector<cv::Point3f> readPoints(std::string path) {
        std::vector<cv::Point3f> pts;
        xml::FileParser parser;
        if (!parser.parseFile((td::String)path)) {
            return pts;
        }
        auto root = parser.getRootNode();
        if (root->getName().cCompare("Points") != 0) return pts;
        auto param = root.getChildNode("Point");
        while (param.isOk()) {
            double x, y, z;
            param.getAttribValue("X", x);
            param.getAttribValue("Y", y);
            param.getAttribValue("Z", z);
            cv::Point3f pom(x, y, z);
            pts.push_back(pom);
            ++param;
        }
        return pts;
    }

    bool onClick(gui::FileDialog* pFD, td::UINT4 dlgID) override
    {
        if (dlgID == 30)
        {
            td::String pathp=pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                cv::Mat R;
                cv::Rodrigues(r, R);
                height(path, cameraMatrix, dist, R, t);
                return true;
            }
            return true;
        }
        if (dlgID == 40)
        {
            td::String pathp = pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                findLine(path);
                return true;
            }
            return true;
        }
        if (dlgID == 50)
        {
            td::String pathp = pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                cv::Mat R;
                cv::Rodrigues(r, R);
                LaserHeight(path, cameraMatrix, dist, R, t);
                return true;
            }
            return true;
        }

        if (dlgID == 60)
        {
            td::String pathp = pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                auto pts = readPoints(path);
                View2D* pv2d;
                pv2d = new View2D(pts);
                _view.addView(pv2d, tr("2D"), &_2d);
                return true;
            }
            return true;
        }
        return false;
    }
};
