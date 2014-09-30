//
//  UISlider.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 9/26/14.
//
//

#include "UISlider.h"
#include "ofMain.h"
#include "UITriggers.h"

UISlider::UISlider() { }
UISlider::UISlider(string name, bool horizontal, int trackX, int trackY, int trackLength, int handleWidth, int handleHeight) {
    this->name = name;
    
    this->horizontal = horizontal;
    
    this->trackX = trackX;
    this->trackY = trackY;
    
    handlePos = 0.0;
    
    this->trackLength = trackLength;
    
    this->handleWidth = handleWidth;
    this->handleHeight = handleHeight;
}
void UISlider::draw() {
    ofPushStyle();
    
    int endTrackX = 0;
    int endTrackY = 0;
    if (horizontal) {
        endTrackX = trackX + trackLength;
        endTrackY = trackY;
    }
    else {
        endTrackX = trackX;
        endTrackY = trackY + trackLength;
    }
    
    ofSetLineWidth(5);
    ofSetColor(200);
    ofLine(trackX, trackY, endTrackX, endTrackY);
    
    if (isGrabbed())
        ofSetColor(ofColor(255,0,0));
    else
        ofSetColor(ofColor(0,0,255));
    ofRect(getHandleX(), getHandleY(), handleWidth, handleHeight);
    
    ofPopStyle();
}
int UISlider::getHandleX() {
    if (horizontal)
        return (int)(trackX + trackLength * handlePos);
    else
        return (int)(trackX - handleWidth/2);
}
int UISlider::getHandleY() {
    if (horizontal)
        return (int)(trackY - handleHeight/2);
    else
        return (int)(trackY + trackLength * handlePos);
}
bool UISlider::overHandle(int xPos, int yPos) {
    float handleX = getHandleX();
    float handleY = getHandleY();
    
    if (xPos < handleX || xPos > handleX + handleWidth)
        return false;
    
    if (yPos < handleY || yPos > handleY + handleHeight)
        return false;
    
    return true;
}

void UISlider::mousePressed(int x, int y) {
    grabbed = true;
    
    mouseOffsetX = x - getHandleX();
    mouseOffsetY = y - getHandleY();
}

void UISlider::mouseDragged(int x, int y) {
    if (!grabbed)
        return;
    
    if (horizontal) {
        int newHandleX = x - mouseOffsetX;
        
        // constrain handle x within the track
        newHandleX = ofClamp(newHandleX, trackX, trackX + trackLength - handleWidth);
        
        handlePos = (double)(newHandleX - trackX) / trackLength;
    }
    else { // vertical track
        int newHandleY = y - mouseOffsetY;
        
        // constrain handle y within the track
        newHandleY = ofClamp(newHandleY, trackY, trackY + trackLength - handleHeight);
        
        handlePos = (double)(newHandleY - trackY) / trackLength;
    }
    UITriggers::sliderTrigger(this);
}


void UISlider::mouseReleased() {
    grabbed = false;
}

bool UISlider::isGrabbed() {
    return grabbed;
}