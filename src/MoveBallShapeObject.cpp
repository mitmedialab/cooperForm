//
//  MoveBallShapeObject.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 10/13/14.
//
//

#include "MoveBallShapeObject.h"


MoveBallShapeObject::MoveBallShapeObject() {
    depthBuffer.allocate(100,100);
}

void MoveBallShapeObject::update() {
    if (ofGetElapsedTimeMillis() - startAnimationTime > animationTime) {
        depthBuffer.begin();
        ofBackground(0);
        depthBuffer.end();
        return;
    }
    float time = (float)(ofGetElapsedTimeMillis() - startAnimationTime) / animationTime;
    
    
    float baseRadius = 4 + (1.0-time) * depthBuffer.getWidth()*0.6;
    
    
    int w = depthBuffer.getWidth();
    int h = depthBuffer.getHeight();
    
    depthBuffer.begin();
    ofBackground(0,0,0);
    
    ofPushStyle();
    
    ofSetColor(255);
    ofCircle(w/2, h/2, baseRadius+8, baseRadius+8);
    
    ofSetColor((255*3)/4);
    ofCircle(w/2, h/2, baseRadius+6, baseRadius+6);
    
    ofSetColor((255*2)/4);
    ofCircle(w/2, h/2, baseRadius+4, baseRadius+4);
    
    ofSetColor(255/4);
    ofCircle(w/2, h/2, baseRadius+2, baseRadius+2);
    
    ofSetColor(0);
    ofCircle(w/2, h/2, baseRadius, baseRadius);
    
    ofPopStyle();
    
    depthBuffer.end();
}

void MoveBallShapeObject::renderTangibleShape(int w, int h) {
    depthBuffer.draw(0,0, w,h);
}
void MoveBallShapeObject::renderTouchScreenGraphics(int w, int h) {
    depthBuffer.draw(0,0, w,h);
}

void MoveBallShapeObject::moveBallToCenter() {
    startAnimationTime = ofGetElapsedTimeMillis();
    toCorner = false;
}
void MoveBallShapeObject::moveBallToCorner() {
    startAnimationTime = ofGetElapsedTimeMillis();
    toCorner = true;
}