//
//  TouchShapeObject.cpp
//  basicExample
//
//  Created by Ken Nakagaki on 6/16/15.
//
//

#include "TouchShapeObject.h"


TouchShapeObject::TouchShapeObject()
{
    mOutputShapeImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    allPixels = new unsigned char[RELIEF_SIZE];
};

void TouchShapeObject::setup()
{
    
}

//----------------------------------------------------

void TouchShapeObject::update() {
    update(0.1);
}

void TouchShapeObject::update(float dt)
{
    unsigned char * pixels;

    //ofxCvGrayscaleImage smallerImage;
    smallerImage.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
    
    int x = RELIEF_SIZE_X / 2;
    int y = RELIEF_SIZE_Y / 2;
    int lineSize = sizeof(char) * RELIEF_SIZE_Y;
    unsigned char controlPinHeight = pinHeightsFromDisplay[x * lineSize + y];
    cpHeight = controlPinHeight;
    cout << "height: "<< (int) controlPinHeight << endl;

    /*
    // depress pin heights depending on their distance from the control pin
    for (int x = 0; x < RELIEF_PHYSICAL_SIZE_X; x++) {
        for(int y = 0; y < RELIEF_PHYSICAL_SIZE_Y; y++){
            int distance = ofDist(RELIEF_PHYSICAL_SIZE_X/2, RELIEF_PHYSICAL_SIZE_Y/2, x, y);
            distance = min(15, distance);
            int outputHeight = ofMap(distance, 0, 15, (int) controlPinHeight, 230);
            allPixels[RELIEF_PHYSICAL_SIZE_X * y + x] = outputHeight;
        }
    }

    // but force the control pin back up
    allPixels[RELIEF_PHYSICAL_SIZE_X * RELIEF_PHYSICAL_SIZE_Y/2 + RELIEF_PHYSICAL_SIZE_X/2] = 230;
     */

    for (int x = 0; x < RELIEF_PHYSICAL_SIZE_X; x++) {
        for(int y = 0; y < RELIEF_PHYSICAL_SIZE_Y; y++){
            int distance = ofDist(RELIEF_PHYSICAL_SIZE_X/2, RELIEF_PHYSICAL_SIZE_Y/2, x, y);
            distance = min(15, distance);
            int outputHeight = ofMap(distance, 0, 15, (int) controlPinHeight, 230);
            allPixels[RELIEF_PHYSICAL_SIZE_X * y + x] = (x + y) * 10 % 128;
        }
    }
}

//----------------------------------------------------

void TouchShapeObject::renderShape()
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void TouchShapeObject::renderTangibleShape(int w, int h)
{
    ofSetColor((int) cpHeight);
    ofCircle(w/2, h/2, w/2);

    ofSetColor((int) cpHeight + 50);
    ofCircle(w/2, h/2, w/20);

    return;

    mOutputShapeImage.draw(0, 0, w, h);
    //ofFbo pinHeightMapImage; //FBO where we render height map
    //pinHeightMapImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    for (int x = 0; x < RELIEF_PHYSICAL_SIZE_X; x++) {
        for(int y = 0; y < RELIEF_PHYSICAL_SIZE_Y; y++){
            int distance = ofDist(RELIEF_PHYSICAL_SIZE_X/2, RELIEF_PHYSICAL_SIZE_Y/2, x, y);
            distance = min(15, distance);
            //int outputHeight = ofMap(distance, 0, 15, (int) controlPinHeight, 230);
            allPixels[RELIEF_PHYSICAL_SIZE_X * y + x] = (x + y) * 10 % 128;
        }
    }
}

//----------------------------------------------------

void TouchShapeObject::renderGraphics(int x, int y, int w, int h)
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void TouchShapeObject::drawGuiScreen(int x, int y, int w, int h)
{
    
}

//----------------------------------------------------

unsigned char* TouchShapeObject::getPixels()
{
    return allPixels;
}