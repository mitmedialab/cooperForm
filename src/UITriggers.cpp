//
//  UITriggers.cpp
//  cooperFORM
//
//  Created by Jared Counts on 9/24/14.
//
//

// All of the table-specific functions triggered by UI events

#include "UITriggers.h"
#include "UIButton.h"


void UITriggers::buttonTrigger(UIButton *button) {
    string name = button->getName();
    if (name == "telepresence") {
        reliefApplication->setMode("telepresence");
        uiHandler->select(button);
    }
    else if (name == "wavy") {
        reliefApplication->setMode("wavy");
        uiHandler->select(button);
    }
    else if (name == "3D") {
        reliefApplication->setMode("3D");
        uiHandler->select(button);
    }
    else if (name == "math") {
        reliefApplication->setMode("math");
        uiHandler->select(button);
    }
}

// initialize relevant static variables
ReliefApplication* UITriggers::reliefApplication;
UIHandler* UITriggers::uiHandler;

void UITriggers::registerReliefApplication(ReliefApplication* relief) {
    UITriggers::reliefApplication = relief;
    UITriggers::uiHandler = &relief->uiHandler;
}