//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ViewTestGL2.h"
#include "ToolBar.h";

class MainWindow : public gui::Window
{
private:
protected:
    ToolBar _mainToolBar;
    MenuBar _mainMenuBar;
    ViewTestGL2 _viewTestGL2;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        //_mainToolBar.showLabels(false);
        _mainToolBar.setIconSize(gui::ToolBar::IconSize::Regular);
        setToolBar(_mainToolBar);
        _mainMenuBar.setAsMain(this);
        setCentralView(&_viewTestGL2);
    }
};
