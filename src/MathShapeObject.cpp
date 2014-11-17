//
//  MathShapeObject.cpp
//  cooperFORM
//
//  Created by Philipp Schoessler on 10/19/14.
//
//

#include "MathShapeObject.h"

MathShapeObject::MathShapeObject() {
    touchscreenImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    pinHeightMapImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    projectorImage.allocate(PROJECTOR_RAW_RESOLUTION_X, PROJECTOR_RAW_RESOLUTION_Y, GL_RGBA);
}

//--------------------------------------------------------------

void MathShapeObject::update() {
    
    float dt = 1.0f / ofGetFrameRate();
    function.update(dt);
    
    //draw in FBOs
    pinHeightMapImage.begin();
    ofPushStyle();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y);
    glOrtho(0.0, TOUCHSCREEN_VISIBLE_SIZE_X, 0, TOUCHSCREEN_SIZE_Y, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ofSetColor(ofColor::white);
    function.drawHeightMap(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y);
    ofPopStyle();
    pinHeightMapImage.end();
    
    ofPixels functionPixels;
    pinHeightMapImage.getTextureReference().readToPixels(functionPixels);
    touchscreenImage.begin();
    ofEnableDepthTest();
    ofPushStyle();
    ofTranslate(570, 570, 570);
    ofRotate(67, 1,0,0);
    ofRotate(67, 0,0,1);
    
    //    cout << ofGetMouseX() << " " << ofGetMouseY() << endl;
    ofEnableLighting();
    ofLight point;
    ofSetColor(255);
    
    point.setPointLight();
    point.setPosition(0,0,100);
    
    point.enable();
    
    ofBackground(255);
    ofSetColor(50);
    //    ofLine(-5,      -5,      -5,  2*60+5,-5,      -5);
    //    ofLine(2*60+5,-5,      -5,  2*60+5,2*60+5,-5);
    //    ofLine(2*60+5,2*60+5,-5,  -5,      2*60+5,-5);
    //    ofLine(-5,      2*60+5,-5,  -5,      -5,      -5);
    //
    //    ofLine(-5,      -5,    55,  2*60+5,-5,    55);
    //    ofLine(2*60+5,-5,      55,  2*60+5,2*60+5,55);
    //    ofLine(2*60+5,2*60+5,  55,  -5,      2*60+5,55);
    //    ofLine(-5,      2*60+5,55,  -5,      -5,    55);
    
    for (int x = 0; x < 60; x++) {
        for (int y = 0; y < 60; y++) {
            int heightmapX = x * functionPixels.getWidth() /60;
            int heightmapY = y * functionPixels.getHeight()/60;
            float height = functionPixels.getColor(heightmapX, heightmapY).getBrightness()/5.f;
            if (x != 0) {
                int adjHeightmapX = (x-1) * functionPixels.getWidth() /60;
                float adjHeight = functionPixels.getColor(adjHeightmapX, heightmapY).getBrightness()/5.f;
                ofLine(2*x,2*y,height, 2*(x-1),2*y,adjHeight);
            }
            if (y != 0) {
                int adjHeightmapY = (y-1) * functionPixels.getHeight() /60;
                float adjHeight = functionPixels.getColor(heightmapX, adjHeightmapY).getBrightness()/5.f;
                ofLine(2*x,2*y,height, 2*x,2*(y-1),adjHeight);
            }
            
            // draw boxes instead
            //            ofDrawBox(x*5- 12*5, y*5 - 12*5, height/2, // position
            //                      5,5,height); // size1
        }
    }
    ofDisableLighting();
    ofPopStyle();
    ofDisableDepthTest();
    touchscreenImage.end();
    
    projectorImage.begin();
    ofPushStyle();
    ofBackground(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    glOrtho(0.0, RELIEF_PROJECTOR_SIZE_X, 0, RELIEF_PROJECTOR_SIZE_Y, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    ofSetColor(ofColor::white);
    function.drawGraphics(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    ofPopStyle();
    projectorImage.end();
}

//--------------------------------------------------------------

void MathShapeObject::renderProjectorOverlay(int w, int h) {
    //projectorImage.draw(273, 178, 1639, 1254); //TODO find proper x y vals
    projectorImage.draw(510, 242, 3151, 1778);
}

//--------------------------------------------------------------

void MathShapeObject::renderTangibleShape(int w, int h) {
    pinHeightMapImage.draw(0, 0, w, h);
}

//--------------------------------------------------------------

void MathShapeObject::renderTouchscreenGraphics(int w, int h) {
    touchscreenImage.draw(0, 0);
}

//--------------------------------------------------------------

void MathShapeObject::nextFunction() {
    function.nextFunction("next",0);
}

//--------------------------------------------------------------

void MathShapeObject::chooseFunction(int func) {
    function.nextFunction("button",func);
}

//--------------------------------------------------------------

void MathShapeObject::modifyVal1Up() {
    function.adjustVar(1,1);
}

//--------------------------------------------------------------

void MathShapeObject::modifyVal1Down() {
    function.adjustVar(1,-1);
}

//--------------------------------------------------------------

void MathShapeObject::modifyVal2Up() {
    function.adjustVar(2,1);
}

//--------------------------------------------------------------

void MathShapeObject::modifyVal2Down() {
    function.adjustVar(2,-1);
}

//--------------------------------------------------------------

void MathShapeObject::reset() {
    function.setEqVal1(1);
    function.setEqVal2(1);
}

//--------------------------------------------------------------

string MathShapeObject::getEq(){
    return function.getEq();
}

//--------------------------------------------------------------

ofImage* MathShapeObject::getEqImage(){
    return function.getEqImage();
}

//--------------------------------------------------------------

string MathShapeObject::getEqVal1(){
    return function.getEqVal1();
}

//--------------------------------------------------------------

string MathShapeObject::getEqVal2(){
    return function.getEqVal2();
}

//--------------------------------------------------------------

vector<OffsetAndFont> MathShapeObject::getVal1XOffsets() {
    return function.getVal1XOffsets();
}

//--------------------------------------------------------------

vector<OffsetAndFont> MathShapeObject::getVal2XOffsets() {
    return function.getVal2XOffsets();
}