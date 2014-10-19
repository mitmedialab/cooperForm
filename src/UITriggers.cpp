//
//  UITriggers.cpp
//  cooperFORM
//
//  Created by Jared Counts on 9/24/14.
//
//

// All of the table-specific functions triggered by UI events


#include "UITriggers.h"

void UITriggers::buttonTrigger(UIButton *button) {
    UIHandler *ui = uiHandler;
    string name = button->getName();
    if (ofGetElapsedTimeMillis() - UITriggers::lastModeSwitched > UITriggers::modeSwitchTimeLimit) {
        if (name == "telepresence") {
            reliefApplication->setMode("telepresence");
            uiHandler->select(button);
            uiHandler->showUIGroup("telepresence");
            uiHandler->hideUIGroup("wavy");
            uiHandler->hideUIGroup("3D");
            uiHandler->hideUIGroup("math");
            uiHandler->hideUIGroup("info");
            UITriggers::lastModeSwitched = ofGetElapsedTimeMillis();
            reliefApplication->showInfo = false;
        }
        else if (name == "wavy") {
            reliefApplication->setMode("wavy");
            uiHandler->select(button);
            uiHandler->hideUIGroup("telepresence");
            uiHandler->showUIGroup("wavy");
            uiHandler->hideUIGroup("3D");
            uiHandler->hideUIGroup("math");
            uiHandler->hideUIGroup("info");
            UITriggers::lastModeSwitched = ofGetElapsedTimeMillis();
            reliefApplication->showInfo = false;
        }
        else if (name == "3D") {
            reliefApplication->setMode("3D");
            uiHandler->select(button);
            uiHandler->hideUIGroup("telepresence");
            uiHandler->hideUIGroup("wavy");
            uiHandler->showUIGroup("3D");
            uiHandler->hideUIGroup("math");
            uiHandler->hideUIGroup("info");
            UITriggers::lastModeSwitched = ofGetElapsedTimeMillis();
            reliefApplication->showInfo = false;
        }
        else if (name == "math") {
            reliefApplication->setMode("math");
            uiHandler->select(button);
            uiHandler->hideUIGroup("telepresence");
            uiHandler->hideUIGroup("wavy");
            uiHandler->hideUIGroup("3D");
            uiHandler->showUIGroup("math");
            uiHandler->hideUIGroup("info");
            UITriggers::lastModeSwitched = ofGetElapsedTimeMillis();
            reliefApplication->showInfo = false;
        }
        else if (name == "modelSelectLeft")
        {
            reliefApplication->threeDShapeObject->changeModel("left");
            cout << "modelSelectLeft" <<endl;
        }
        else if (name == "modelSelectRight")
        {
            reliefApplication->threeDShapeObject->changeModel("right");
            cout << "modelSelectRight" <<endl;
        }
    }
}
void UITriggers::buttonUnselect(UIButton *button) {
    string name = button->getName();
    if (name == "telepresence"  || name == "wavy" || name == "3D" || name == "math") {
        uiHandler->unselect(button);
        uiHandler->hideUIGroup(name);
        uiHandler->showUIGroup("info");
    }
}

void UITriggers::sliderTrigger(UISlider *slider) {
    string name = slider->getName();
    if (name == "sliderScale") {
        reliefApplication->threeDShapeObject->setScale(slider->getVal(0, 1));
    }
    //cout << slider->getVal() << endl;
}

// initialize relevant static variables
ReliefApplication* UITriggers::reliefApplication;
UIHandler* UITriggers::uiHandler;
int UITriggers::modeSwitchTimeLimit;
long UITriggers::lastModeSwitched;

void UITriggers::registerReliefApplication(ReliefApplication* relief) {
    UITriggers::reliefApplication = relief;
    UITriggers::uiHandler = relief->uiHandler;
    
    // initialize these too
    // since C++ spec won't let us initialize static members in-line the .h file
    UITriggers::modeSwitchTimeLimit = 300;
    UITriggers::lastModeSwitched = 0;
}