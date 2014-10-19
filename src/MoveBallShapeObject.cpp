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
        if (!toCenter && !ballInCorner)
            ballInCorner = true;
        return;
    }
    float maxHeightOfEdge = 200;
    float minRadius = 5.5;
    float time = (float)(ofGetElapsedTimeMillis() - startAnimationTime) / animationTime;
    
    float toCenterTimeFraction;
    if (toCenter) {
        toCenterTimeFraction = 1;
        time = 1.0 - time; // reverse time
    }
    else
        toCenterTimeFraction = 0.6;
    
    if (time < toCenterTimeFraction) {
        float toCenterTime = 1.0 - time / toCenterTimeFraction;
        float tScaled = (1.0 - cos(pi * toCenterTime)) / 2;
        float baseRadius = minRadius + tScaled * depthBuffer.getWidth()*0.55;
        
        int w = depthBuffer.getWidth();
        int h = depthBuffer.getHeight();
        
        int toX = w/2;
        int toY = h/2;

        depthBuffer.begin();
        ofBackground(0,0,0);
        ofPushStyle();
        
        ofSetColor(maxHeightOfEdge);
        ofCircle(toX, toY, baseRadius+8, baseRadius+8);
        
        ofSetColor((maxHeightOfEdge*3)/4);
        ofCircle(toX, toY, baseRadius+6, baseRadius+6);
        
        ofSetColor((maxHeightOfEdge*2)/4);
        ofCircle(toX, toY, baseRadius+4, baseRadius+4);
        
        ofSetColor(maxHeightOfEdge/4);
        ofCircle(toX, toY, baseRadius+2, baseRadius+2);
        
        ofSetColor(0);
        ofCircle(toX, toY, baseRadius, baseRadius);


        ofPopStyle();
        depthBuffer.end();
    }
    else {
        float toCornerTime = (time - toCenterTimeFraction) / (1.0 - toCenterTimeFraction);
        float tScaled = (1.0 - cos(pi * toCornerTime)) / 2;
        
        float baseRadius = minRadius;
        
        depthBuffer.begin();
        ofBackground(0,0,0);
        ofPushStyle();
        
        int w = depthBuffer.getWidth();
        int h = depthBuffer.getHeight();
        
        int fromX = w/2;
        int fromY = h/2;
        
        int toX = w-15;
        int toY = 15;
        
        float x = fromX + (fromX - toX) * tScaled;
        float y = fromY + (fromY - toY) * tScaled;
        
        ofSetColor(maxHeightOfEdge);
        ofCircle(x, y, baseRadius+8, baseRadius+8);
        
        ofSetColor((maxHeightOfEdge*3)/4);
        ofCircle(x, y, baseRadius+6, baseRadius+6);
        
        ofSetColor((maxHeightOfEdge*2)/4);
        ofCircle(x, y, baseRadius+4, baseRadius+4);
        
        ofSetColor(maxHeightOfEdge/4);
        ofCircle(x, y, baseRadius+2, baseRadius+2);
        
        ofSetColor(0);
        ofCircle(x, y, baseRadius, baseRadius);
        
        
        ofPopStyle();
        depthBuffer.end();
    }
}

void MoveBallShapeObject::renderTangibleShape(int w, int h) {
    depthBuffer.draw(0,0, w,h);
}
void MoveBallShapeObject::renderTouchscreenGraphics(int w, int h) {
    depthBuffer.draw(0,0, w,h);
}

void MoveBallShapeObject::moveBallToCorner() {
    startAnimationTime = ofGetElapsedTimeMillis();
    toCenter = false;
}
void MoveBallShapeObject::moveBallToCenter() {
    startAnimationTime = ofGetElapsedTimeMillis();
    ballInCorner = false;
    toCenter = true;
}
bool MoveBallShapeObject::isBallInCorner() {
    return ballInCorner;
}