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
#include "UIButton.h"
#include "UISlider.h"
#include "UIImage.h"
#include "ofMain.h"

class UIHandler {
public:
    UIHandler();
    
    void draw();
    
    void addButton(UIButton *button);
    void addButtonGroup(vector<UIButton*> group);
    
    void addImage(UIImage *image);
    
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y);
    void mouseDragged(int x, int y);
    
    void select(UIButton *button);
private:
    vector<UIButton*> buttons;
    vector<UISlider*> sliders;
    vector<UIImage*> images;
    
    // groups of button
    // for selection and unselection
    vector< vector<UIButton*> > buttonGroups;
};


#endif /* defined(__cooperFORM__UIHandler__) */
