//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>

class ToolBar : public gui::ToolBar
{
    using Base = gui::ToolBar;
protected:
    gui::Image _camera;
    gui::Image _2d;
    gui::Image _3d;
    gui::Image _scatter;
    gui::Image _target;
    gui::Image _height;
    gui::Image _laser;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 2, 0, true)
    , _camera(":camera")
    , _2d(":2d")
    , _laser(":laser")
    , _3d(":3d")
    , _scatter(":scatter")
    , _target(":target")
    , _height(":height")
    {
        //showLabels(false);
        addItem(tr("Calibration"), &_target, tr(""), 20, 0, 0, 10);
        addItem(tr("Camera"), &_camera, tr(""), 20, 0, 0, 20);
        addItem(tr("PointHeight"), &_height, tr(""), 20, 0, 0, 30);
        addItem(tr("LineDetection"), &_laser, tr(""), 20, 0, 0, 40);
        addItem(tr("LineHeight"), &_scatter, tr(""), 20, 0, 0, 50);
        addItem(tr("2D"), &_2d, tr(""), 20, 0, 0, 60);
        addItem(tr("3D"), &_3d, tr(""), 20, 0, 0, 70);
    }
    
    void forwardMessagesTo(gui::IMessageConsumer* pConsumer)
    {
        Base::forwardMessagesTo(pConsumer);
    }
    
};
