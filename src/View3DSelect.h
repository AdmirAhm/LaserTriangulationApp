#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <cnt/SafeFullVector.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/ColorPicker.h>
#include <gui/CheckBox.h>
#include <gui/NumericEdit.h>
#include "td/Variant.h"

class View3DSelect : public gui::View {
protected:
    gui::GridLayout _gl;

    gui::Label _lbldist;

    gui::NumericEdit _dist;

public:
    View3DSelect()
        : _gl(1, 2)
        , _lbldist(tr("Distance between lines:"))
        , _dist(td::real8)
    {
        gui::GridComposer gc(_gl);
        gc.appendRow(_lbldist);
        gc.appendCol(_dist);
        setLayout(&_gl);
    }

    ~View3DSelect(){}

    void getVals(double& dist) {
        dist = _dist.getValue().r8Val();
    }

};
