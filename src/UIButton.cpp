//
//  UIButton.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 9/23/14.
//
//

#include "UIButton.h"
#include "UITriggers.h"
#include "ofMain.h"


// temporary constructor
UIButton::UIButton() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    name = "";
}

UIButton::UIButton(string name, int xPos, int yPos, int w, int h) {
    this->name = name;
    x = xPos;
    y = yPos;
    width = w;
    height = h;
}

string UIButton::getName() {
    return name;
}

void UIButton::draw() {
    if (isPressed())
        ofSetColor(ofColor(255,0,0));
    else
        ofSetColor(ofColor(0,0,255));
    
    ofRect(x,y, width, height);
}


// given x and y coordinates
// return whether it is within this button
bool UIButton::overButton(int xPos, int yPos) {
    if (xPos < x || xPos > x+width)
        return false;
    if (yPos < y || yPos > y+height)
        return false;
    
    return true;
}

void UIButton::mousePressed() {
    pressed = true;
}
void UIButton::mouseReleased() {
    pressed = false;
    UITriggers::buttonTrigger(name);
}

// triggered when the mouse has left the button,
// but didn't release
void UIButton::mouseLeft() {
    
    pressed = false;
}
bool UIButton::isPressed() {
    return pressed;
}