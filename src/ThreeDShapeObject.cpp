//
//  ThreeDShapeObject.cpp
//  cooperFORM
//
//  Created by Philipp Schoessler on 10/17/14.
//
//

#include "ThreeDShapeObject.h"


ThreeDShapeObject::ThreeDShapeObject() {
    
    model.loadModel("models/beetle3.3ds");
    //vbo = model.getMesh(0);
    bgImg.loadImage("test.jpg");
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //ofEnableDepthTest();
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
    
    fbo.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    
    shader.load("shaders/heightMapShader.vert", "shaders/heightMapShader.frag");
    //cam.setPosition(1920/4, 1080/2, 255);
    
    dampen = 1.3;
    
}

//--------------------------------------------------------------

void ThreeDShapeObject::update() {
    //model.setRotation(1, 270 + ofGetElapsedTimef() * 60, 1, 0, 1);

    fbo.begin();
        ofClear(255,255,255);
        //bgImg.draw(0, 0);
        //cam.begin();

        //glEnable(GL_DEPTH_TEST);
        shader.begin();
            model.setPosition((1920 - 840) / 2, (1080)/2, 0);
            model.setRotation(1, 90, 1, 0, 0);
            //model.setScale(0.45, 0.45, 0.45);
            model.drawFaces();
        shader.end();
        glDisable(GL_DEPTH_TEST);
        //cam.end();
    fbo.end();
}

//--------------------------------------------------------------

void ThreeDShapeObject::renderProjectorOverlay(int w, int h) {

}

//--------------------------------------------------------------

void ThreeDShapeObject::renderTangibleShape(int w, int h) {
    
}

//--------------------------------------------------------------

void ThreeDShapeObject::renderTouchScreenGraphics(int w, int h) {
    
    ofBackground(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y);
    
    glOrtho(0.0, TOUCHSCREEN_VISIBLE_SIZE_X, 0, TOUCHSCREEN_SIZE_Y, -500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    shader.begin();
        model.setPosition((1920 - 840) / 2, (1080)/2, -150);
        //model.setRotation(1, 90, 1, 0, 0);
        model.setRotation(1, -1 * angle, axis.x, axis.y, axis.z);
        model.setScale(1, 1, 1);
        model.drawFaces();
    shader.end();
    glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------

void ThreeDShapeObject::renderMarginGraphics(int x, int y)
{
    marginX = x;
    marginY = y;
    quat.getRotate(angle, axis);
    
    ofSetColor(255);
    ofRect(x, y, MARGIN_X, MARGIN_X);
    ofPushMatrix();
        ofTranslate(x + MARGIN_X/2,y + MARGIN_X/2);
        glEnable(GL_DEPTH_TEST);
        model.setPosition(0, 0, 0);
        model.setRotation(1, angle, axis.x, axis.y, axis.z);
        model.setScale(0.4, 0.4, 0.4);
        model.drawWireframe();
    ofPopMatrix();
}

//--------------------------------------------------------------

void ThreeDShapeObject::reset() {
    cout << "reset" << endl;
}

//--------------------------------------------------------------

void ThreeDShapeObject::setMouseDragInfo(int x, int y, int button)
{
    if(ofDist(x, y, marginX + MARGIN_X/2, marginY +MARGIN_X/2) < MARGIN_X/2)
    {
        ofVec2f mouse(x, y);
        ofQuaternion yRot((x - oldMouse.x) * dampen, ofVec3f(0, 1, 0));
        ofQuaternion xRot((y - oldMouse.y) * dampen, ofVec3f(-1, 0, 0));
        quat *= yRot*xRot;
        oldMouse = mouse;
    }
}

//--------------------------------------------------------------

void ThreeDShapeObject::setMousePressedInfo(int x, int y)
{
    oldMouse = ofVec2f(x,y); //prevents popping
}
