//
//  KinectShapeObject.cpp
//  basicExample
//
//  Created by Daniel Leithinger on 3/20/14.
//
//

#include "KinectShapeObject.h"

KinectShapeObject::KinectShapeObject() {
    mKinectHeightImage.allocate(300,300); //KINECT_X, KINECT_Y);
    mOutputShapeImage.allocate(300,300); //KINECT_X, KINECT_Y);
};

void KinectShapeObject::update(){

    mKinectHeightImage.begin();
    mKinectTracker->drawDepthThreshedImage(0,0, mKinectHeightImage.getWidth(), mKinectHeightImage.getHeight());
    mKinectHeightImage.end();

    mOutputShapeImage.begin();
    mKinectTracker->drawColorImage(0,0, mOutputShapeImage.getWidth(), mOutputShapeImage.getHeight());
    mOutputShapeImage.end();

};

void KinectShapeObject::renderVerticalScreenGraphics(int w, int h){
    mOutputShapeImage.draw(0,0, w, h);
};

void KinectShapeObject::renderTouchscreenGraphics(int w, int h){
    mOutputShapeImage.draw(0,0, w, h);
    //mOutputShapeImage.draw(0,0, w, h);
};

void KinectShapeObject::renderTangibleShape(int w, int h){
    mKinectHeightImage.draw(0,0, w, h);
};


void KinectShapeObject::mouseMoved(int x, int y){

};
void KinectShapeObject::mousePressed(int x, int y, int button){

};

void KinectShapeObject::setTableValuesForShape(ShapeIOManager *pIOManager) {
    pIOManager->set_max_speed(200);
    pIOManager->set_gain_p(1.5f);
    pIOManager->set_gain_i(0.045f);
    pIOManager->set_max_i(25);
    pIOManager->set_deadzone(2);
};