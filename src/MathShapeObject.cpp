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
    pinHeightMapImageSmall.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y, GL_RGBA);
    projectorImage.allocate(PROJECTOR_RAW_RESOLUTION_X, PROJECTOR_RAW_RESOLUTION_Y, GL_RGBA);
    
}

//--------------------------------------------------------------

void MathShapeObject::update() {
    
    float dt = 1.0f / ofGetFrameRate();
    function.update(dt);
    
    //draw in FBOs
    touchscreenImage.begin();
    ofPushStyle();
    ofBackground(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y);
    glOrtho(0.0, TOUCHSCREEN_VISIBLE_SIZE_X, 0, TOUCHSCREEN_SIZE_Y, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ofSetColor(ofColor::white);
    function.drawGraphics(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y);
    ofPopStyle();
    touchscreenImage.end();
    
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
    
    pinHeightMapImageSmall.begin();
    ofPushStyle();
    ofBackground(0);
    ofSetColor(255);
    pinHeightMapImage.draw(0, 0, RELIEF_SIZE_X, RELIEF_SIZE_Y);
    ofPopStyle();
    pinHeightMapImageSmall.end();
    
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

}

//--------------------------------------------------------------

void MathShapeObject::renderTangibleShape(int w, int h) {

}

//--------------------------------------------------------------

void MathShapeObject::renderTouchscreenGraphics(int w, int h) {
    //touchscreenImage.draw(0, 0);
    //pinHeightMapImage.draw(0, 0);
    pinHeightMapImageSmall.draw(0, 0);
    //projectorImage.draw(0, 0);
}

//--------------------------------------------------------------

void MathShapeObject::nextFunction() {
    function.nextFunction(true);
}

//--------------------------------------------------------------

