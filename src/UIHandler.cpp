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
    buttonGroups = vector< vector<UIButton*> >();
    images = vector<UIImage*>();
}

void UIHandler::draw() {
    
    for (UIButton *button : buttons)
        button->draw();
    for (UIImage *image : images)
        image->draw();
    
    ofSetColor(ofColor(255));
}

void UIHandler::addButton(UIButton *button) {
    buttons.push_back(button);
}

void UIHandler::addButtonGroup(vector<UIButton*> group) {
    buttonGroups.push_back(group);
}

void UIHandler::addImage(UIImage* image) {
    images.push_back(image);
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

void UIHandler::select(UIButton *button) {
    // find out which groups it's in
    for (vector<UIButton*> buttonGroup : buttonGroups) {
        std::vector<UIButton*>::iterator it;
        it = find(buttonGroup.begin(), buttonGroup.end(), button);
        if (it != buttonGroup.end()) {
            
            // found the button!
            // now deselect every other button in this group
            for (UIButton *other : buttonGroup) {
                if (other != button)
                    other->unselect();
            }
            
        }
        // look for more groups this button might belong in
    }
    
    // finally select the button
    button->select();
}