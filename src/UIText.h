//
//  UIText.h
//  cooperFORM
//
//  Created by Tangible Media Group on 10/9/14.
//
//

#ifndef __cooperFORM__UIText__
#define __cooperFORM__UIText__

#include "Constants.h"
#include "UIElement.h"
#include "ofMain.h"
#include "ofxFTGL.h"

using namespace std;

class UIText  : public UIElement {
public:
    UIText() { };
    UIText(string text, string name, int size, int xPos, int yPos);
    
    void draw();
    
    string getName();
    
    void setWidth(int width);
    void setAlignment(string alignment);
    void setColor(ofColor color);
    
    void enable() { };
    void disable() { };
    
    void show();
    void hide();
    
    // since there's no other way to get a UIText's height
    float getHeight();
private:
    string text;
    string name;
    ofxFTGLSimpleLayout layout;
    int x, y;
    ofColor color;
    bool visible = true;
};

#endif /* defined(__cooperFORM__UIText__) */
