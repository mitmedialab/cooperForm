//
//  UIImage.h
//  cooperFORM
//
//  Created by Tangible Media Group on 9/29/14.
//
//

#ifndef __cooperFORM__UIImage__
#define __cooperFORM__UIImage__

#include "UIElement.h"
#include "ofMain.h"

class UIImage : public UIElement {
public:
    UIImage(string imagePath, int x, int y);
    UIImage(ofImage *image, int x, int y);
    
    void draw();
    string getName();
    
    // inherited from UIElement
    // does nothing since UIImage is not interactive
    void enable() { };
    void disable() { };
    
    void show();
    void hide();
private:
    string name;
    ofImage *image;
    int x, y;
    bool visible = true;
};

#endif /* defined(__cooperFORM__UIImage__) */
