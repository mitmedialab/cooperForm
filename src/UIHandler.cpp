//
//  UIHandler.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 9/23/14.
//
//

#include "UIHandler.h"

UIHandler::UIHandler() {
    buttons = vector<UIButton*>();
}

void UIHandler::draw() {
    for (UIButton *button : buttons)
        button->draw();
    ofSetColor(ofColor(255));
}

void UIHandler::addButton(UIButton *button) {
    buttons.push_back(button);
}

void UIHandler::mousePressed(int x, int y) {
    // look for buttons under this mouse position
    for (UIButton *button : buttons) {
        if (button->overButton(x, y))
            button->mousePressed();
    }
}

void UIHandler::mouseReleased(int x, int y) {
    // look for buttons under this mouse position
    for (UIButton *button : buttons) {
        if (button->isPressed()) {
            // check first if the mouse is still over the button
            // to see which event trigger to use
            if (button->overButton(x,y))
                button->mouseReleased();
            else
                button->mouseLeft();
        }
    }
}

void UIHandler::mouseDragged(int x, int y) {
    for (UIButton *button : buttons) {
        if (button->isPressed()) {
            // check to see if the mouse pressed a button but left
            if (!button->overButton(x,y))
                button->mouseLeft();
        }
    }
}