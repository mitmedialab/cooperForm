//
//  UIImage.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 9/29/14.
//
//

#include "UIImage.h"

UIImage::UIImage(string imagePath, int x, int y) {
    image = new ofImage();
    image->loadImage(imagePath);
    this->x = x;
    this->y = y;
}
UIImage::UIImage(ofImage *image, int x, int y) {
    this->image = image;
    
    this->x = x;
    this->y = y;
}

void UIImage::draw() {
    if (visible)
        image->draw(x,y, image->getWidth(), image->getHeight());
}
string UIImage::getName() {
    return name;
}

void UIImage::show() {
    visible = true;
}
void UIImage::hide() {
    visible = false;
}