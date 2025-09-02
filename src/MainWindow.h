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
#include <gui/FileDialog.h>
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
                pv = new ViewCalib(&cameraMatrix, &dist);
                _view.addView(pv, tr("Kalibracija"), &_target);
                return true;
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
            }
        }
        return false;
    }

    bool onClick(gui::FileDialog* pFD, td::UINT4 dlgID) override
    {
        if (dlgID == 30)
        {
            td::String pathp=pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
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
        return false;
    }
};
