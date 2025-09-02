//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu subApp;
    gui::SubMenu subTools;
protected:
    void populateSubToolsMenu()
    {
        auto& items = subTools.getItems();
        items[0].initAsActionItem(tr("Kalibracija"), 10);
        items[1].initAsActionItem(tr("Slikanje"), 20);
        items[2].initAsActionItem(tr("Visina tacke"), 30);
        items[3].initAsActionItem(tr("Pronalazak linije"), 40);
        items[4].initAsActionItem(tr("Visina linije"), 50);
        items[5].initAsActionItem(tr("2D prikaz"), 60);
        items[6].initAsActionItem(tr("3D prikaz"), 70);
        //tems[1] is separator
    }
    void populateSubAppMenu()
    {
        auto& items = subApp.getItems();
        items[0].initAsQuitAppActionItem(tr("Quit"), "q"); //id 0 is used to terminate app
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subApp(10, "App", 1) //allocate items for the Application subMenu
    , subTools(20, "Alati", 7) //allocate items for the Application subMenu
    {
        populateSubAppMenu();
        populateSubToolsMenu();
        _menus[0] = &subApp;
        _menus[1] = &subTools;
    }
};
