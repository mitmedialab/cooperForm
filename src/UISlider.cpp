//
//  UISlider.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 9/26/14.
//
//

#include "UISlider.h"

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
    float handleY = getHandleX();
}