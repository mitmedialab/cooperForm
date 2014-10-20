//
//  ThreeDShapeObject.cpp
//  cooperFORM
//
//  Created by Philipp Schoessler on 10/17/14.
//
//

#include "ThreeDShapeObject.h"


ThreeDShapeObject::ThreeDShapeObject() {
    
    dampen = 1.3;
    idx = 0;
    numLoadedModels = 3;
    for (int i = 0; i < numLoadedModels; i++)
    {
        models.push_back(new threeDModel());
    }
    models.at(0)->model.loadModel("models/beetle3.3ds");
    models.at(0)->name = "Car";
    models.at(1)->model.loadModel("models/NewSquirrel.3ds");
    models.at(1)->name = "Squirrel";
    models.at(2)->model.loadModel("models/cube1.3ds");
    models.at(2)->name = "Cube";
    
    //model.loadModel("models/beetle3.3ds");
    bgImg.loadImage("test.jpg");
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    pinHeightMapImageSmall.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y, GL_RGBA);
    pinHeightMapImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    
    shader.load("shaders/heightMapShader.vert", "shaders/heightMapShader.frag");
 
    currentModelName = models.at(idx)->name;
}

//--------------------------------------------------------------

void ThreeDShapeObject::update() {
    
    pinHeightMapImage.begin();
    ofPushMatrix();
    ofPushStyle();
    ofBackground(0);
    
    //GL stuff
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y);
    glOrtho(0.0, TOUCHSCREEN_VISIBLE_SIZE_X, 0, TOUCHSCREEN_SIZE_Y, -1000, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    
    shader.begin();
    models.at(idx)->model.setPosition((1920 - 840) / 2, (1080)/2, -150);
    models.at(idx)->model.setRotation(1, -1 * angle, axis.x, axis.y, axis.z);
    models.at(idx)->model.setScale(.5 + (.8 * scale), .5 + (.8 * scale), .5 + (.8 * scale));
    models.at(idx)->model.drawFaces();
    shader.end();
    
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
    ofPopStyle();
    pinHeightMapImage.end();
    
    
    pinHeightMapImageSmall.begin();
    pinHeightMapImage.draw(0, 0, RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
    pinHeightMapImageSmall.end();
}

//--------------------------------------------------------------

void ThreeDShapeObject::renderProjectorOverlay(int w, int h) {

}

//--------------------------------------------------------------

void ThreeDShapeObject::renderTangibleShape(int w, int h) {
    pinHeightMapImageSmall.draw(0, 0);
}

//--------------------------------------------------------------

void ThreeDShapeObject::renderTouchscreenGraphics(int w, int h) {
    pinHeightMapImage.draw(0, 0);
}

//--------------------------------------------------------------

void ThreeDShapeObject::renderMarginGraphics(int x, int y)
{
    marginX = x;
    marginY = y;
    quat.getRotate(angle, axis);
    
    ofPushStyle();
    
    //draw background
    ofSetColor(255);
    ofRect(0, 0, MARGIN_X, MARGIN_X);
    
    //draw th 3D model's wireframe
    ofPushMatrix();
        ofTranslate(x + MARGIN_X/2,y + MARGIN_X/2);
        glEnable(GL_DEPTH_TEST);
        models.at(idx)->model.setPosition(0, 0, 0);
        models.at(idx)->model.setRotation(1, angle, axis.x, axis.y, axis.z);
        models.at(idx)->model.setScale(0.2, 0.2, 0.2);
    
        //draw wireframe in black
        ofSetColor(200);
        //glPolygonMode( GL_FRONT, GL_LINE );
        models.at(idx)->model.drawFaces();
        models.at(idx)->model.setScale(0.201, 0.201, 0.201);
        ofSetColor(0);
        models.at(idx)->model.drawWireframe();
        glDisable(GL_DEPTH_TEST);
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------

void ThreeDShapeObject::changeModel(int idx)
{
    if(idx > numLoadedModels)
    {
        cout << "can't change Model: only " << numLoadedModels << "are available" << endl;
        return;
    }
    this->idx = idx;
    currentModelName = models.at(idx)->name;
}

//--------------------------------------------------------------

void ThreeDShapeObject::changeModel(string direction)
{
    if(direction != "left" && direction != "right")
    {
        cout << "changeModel command unknow" << endl;
        return;
    }
    else if(direction == "left")
    {
        idx--;
        if (idx < 0)
            idx = numLoadedModels - 1;
    }
    else
    {
        idx++;
        if (idx >= numLoadedModels) {
            idx = 0;
        }
    }
    currentModelName = models.at(idx)->name;
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

//--------------------------------------------------------------

void ThreeDShapeObject::setScale(float uniformScaleVal)
{
    scale = uniformScaleVal;
}

//--------------------------------------------------------------

string ThreeDShapeObject::getCurrentModelName()
{
    return currentModelName;
}

//--------------------------------------------------------------

