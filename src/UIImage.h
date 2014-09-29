//
//  UIImage.h
//  cooperFORM
//
//  Created by Tangible Media Group on 9/29/14.
//
//

#ifndef __cooperFORM__UIImage__
#define __cooperFORM__UIImage__

#include <iostream>
#include "ofMain.h"

class UIImage {
public:
    UIImage(string imagePath, int x, int y);
    UIImage(ofImage *image, int x, int y);
    void draw();
    
private:
    ofImage *image;
    int x, y;
};

#endif /* defined(__cooperFORM__UIImage__) */
