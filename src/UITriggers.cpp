//
//  UITriggers.cpp
//  cooperFORM
//
//  Created by Jared Counts on 9/24/14.
//
//

// All of the table-specific functions triggered by UI events

#include "UITriggers.h"



void UITriggers::buttonTrigger(string name) {
    if (name == "telepresence") {
        reliefApplication->setMode("telepresence");
    }
    else if (name == "wavy") {
        reliefApplication->setMode("wavy");
    }
    else if (name == "3D") {
        reliefApplication->setMode("3D");
    }
    else if (name == "math") {
        reliefApplication->setMode("math");
    }
}


// initialize this static variable
ReliefApplication* UITriggers::reliefApplication;

void UITriggers::registerReliefApplication(ReliefApplication* relief) {
    UITriggers::reliefApplication = relief;
}