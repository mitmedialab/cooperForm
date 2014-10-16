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
    if (visible) {
        image->draw(x,y, image->getWidth(), image->getHeight());
        if (hasCaption)
            captionText.draw();
    }
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

void UIImage::setCaption(string text, int size, int width) {
    captionText = UIText(text, text, size, x + image->getWidth()/2 - width/2, y + image->getHeight() + 20);
    captionText.setWidth(width);
    captionText.setAlignment("center");
    hasCaption = true;
}