//
//  MoldShapeObject.h
//  basicExample
//
//  Created by Daniel Windham on 6/22/15.
//
//

#ifndef __basicExample__MoldShapeObject__
#define __basicExample__MoldShapeObject__

#include <iostream>
#include "ShapeObject.h"
#include "Constants.h"
#include "ShapeIOManager.h"
#include "ofxOpenCv.h"

    
#define NUM_FILTER_FRAME 6
#define MOLDED_SHAPE_DIM 16

class MoldedShape {
public:
    MoldedShape(int id, int _heightMap[MOLDED_SHAPE_DIM][MOLDED_SHAPE_DIM]) : id(id) {
        for (int i = 0; i < MOLDED_SHAPE_DIM; i++) {
            for (int j = 0; j < MOLDED_SHAPE_DIM; j++) {
                heightMap[i][j] = _heightMap[i][j];
            }
        }
    };
    int getId() {return id;};
    int x = 0;
    int y = 0;
    int heightMap[MOLDED_SHAPE_DIM][MOLDED_SHAPE_DIM];

private:
    int id;
};

class MoldShapeObject : public ShapeObject {

public:
    MoldShapeObject();
    ~MoldShapeObject();
    void setup();
    void update();
    void update(float dt);
    void renderShape();
    void renderTangibleShape(int w, int h);
    void renderGraphics(int x, int y, int w, int h);
    void drawGuiScreen(int x, int y, int w, int h);
    void renderTouchscreenGraphics(int w, int h);
    void setPinHeight(unsigned char pinHeightReceive[RELIEF_SIZE_X][RELIEF_SIZE_Y]){
        mPinHeightReceive = (unsigned char *) pinHeightReceive;
    };
    void ignoreCurrentlyDisplacedPins();
    //void setImageWarper(ImageWarper * pImageWarper) {mImageWarper = pImageWarper;};
    void setTableValuesForShape(ShapeIOManager *pIOManager);
    unsigned char* getPixels();

    int xCoordinateShift(int num);
    

    Boolean isRecording = false;
    
    string get_shape_name() {return shape_name; };
    string shape_name = "Mold";
    
    
private:
    unsigned char *mPinHeightReceive;
   // ImageWarper * mImageWarper;
    ofxCvGrayscaleImage mOutputShapeImage, smallerImage;
    ofImage pinHeightMapImage;
    
    unsigned char* allPixels;
    int differenceHeight[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    int filterFrame = NUM_FILTER_FRAME;
    
    unsigned char allPixels_store[RELIEF_SIZE][NUM_FILTER_FRAME];
    
    Boolean isTouched[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    int holdsObject[RELIEF_SIZE_X][RELIEF_SIZE_Y];

    vector<MoldedShape *> moldedShapes;
    int nextObjectId = 0;
    int getUID() {return nextObjectId++;}
};


#endif /* defined(__basicExample__MoldShapeObject__) */
