#pragma once
#include <gui/Dialog.h>
#include "View3DSelect.h"

class Dialog3DSelect : public gui::Dialog
{
protected:
    View3DSelect _view;
    double* _dist;

    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == Dialog::Button::ID::OK)
        {
            _view.getVals(*_dist);
        }

        return true;
    }
public:
    Dialog3DSelect(gui::Frame* pFrame, td::UINT4 wndID, double* dist)
    : gui::Dialog(pFrame, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                            {gui::Dialog::Button::ID::Cancel, tr("Cancel")}}, gui::Size(350, 150), wndID)
    {
        _dist = dist;
        setTitle(tr("3D View"));
        setCentralView(&_view);
    }

    ~Dialog3DSelect() {
    }

};
