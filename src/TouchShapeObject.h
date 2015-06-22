//
//  TouchShapeObject.h
//  basicExample
//
//  Created by Ken Nakagaki on 6/16/15.
//
//

#ifndef __basicExample__TouchShapeObject__
#define __basicExample__TouchShapeObject__

#include <iostream>
#include "ShapeObject.h"
#include "Constants.h"
#include "ShapeIOManager.h"
#include "ofxOpenCv.h"

class TouchShapeObject : public ShapeObject {
    
public:
    TouchShapeObject();
    void setup();
    void update();
    void update(float dt);
    void renderShape();
    void renderTangibleShape(int w, int h);
    void renderGraphics(int x, int y, int w, int h);
    void drawGuiScreen(int x, int y, int w, int h);
    void setPinHeightsFromDisplayContainer(unsigned char pinHeightsContainer [RELIEF_SIZE_X][RELIEF_SIZE_Y]){
        pinHeightsFromDisplay = (unsigned char *) pinHeightsContainer;
    };
    //void setImageWarper(ImageWarper * pImageWarper) {mImageWarper = pImageWarper;};
    //void setTableValuesForShape(ShapeIOManager *pIOManager);
    unsigned char* getPixels();
    
    string get_shape_name() {return shape_name; };
    string shape_name = "Touch";
    
    
private:
    unsigned char cpHeight;
    unsigned char *pinHeightsFromDisplay;
   // ImageWarper * mImageWarper;
    ofxCvGrayscaleImage mOutputShapeImage, smallerImage;
    ofImage pinHeightMapImage;
    
    unsigned char* allPixels;
};


#endif /* defined(__basicExample__TouchShapeObject__) */