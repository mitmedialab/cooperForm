//
//  UIHandler.h
//  cooperFORM
//
//  Created by Tangible Media Group on 9/23/14.
//
//

#ifndef __cooperFORM__UIHandler__
#define __cooperFORM__UIHandler__

#include <iostream>
#include "UIElement.h"
#include "UIButton.h"
#include "UIText.h"
#include "UISlider.h"
#include "UIImage.h"
#include "ofMain.h"

class UIHandler {
public:
    UIHandler();
    
    void draw();
    
    void addButton(UIButton *button);
    void addButtonGroup(vector<UIButton*> group);
    
    void addSlider(UISlider *slider);
    
    void addImage(UIImage *image);
    
    void addText(UIText *text);
    
    void addUIGroup(vector<UIElement*> uiGroup, string name);
    
    void showUIGroup(string groupName);
    void hideUIGroup(string groupName);
    
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);
    void mouseDragged(int x, int y);
    
    void select(UIButton *button);
private:
    vector<UIButton*> buttons;
    vector<UISlider*> sliders;
    vector<UIImage*> images;
    vector<UIText*> texts;
    
    // groups of button
    // for selection and unselection
    vector< vector<UIButton*> > buttonGroups;
    
    map< string, vector<UIElement*> > uiGroups;
};


#endif /* defined(__cooperFORM__UIHandler__) */
