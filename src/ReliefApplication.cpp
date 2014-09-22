#include "ReliefApplication.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    // initialize communication with the pin display
	mIOManager = new ShapeIOManager();
    
    // setup default valus for pins
    // @todo move to config file?
    gain_P = 0.5;
    gain_I = 0;
    max_I = 60;
    deadZone = 0;
    maxSpeed = 220;
    
    // allocate all the necessary frame buffer objects
    projectorOverlayImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y, GL_RGB);
    pinHeightMapImageSmall.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y, GL_RGB);
    touchScreenDisplayImage.allocate(TOUCHSCREEN_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    verticalDisplayImage.allocate(VERTICAL_DISPLAY_SIZE_X, VERTICAL_DISPLAY_SIZE_Y, GL_RGBA);

    // setup camera interface
    cameraTracker.setup();
    
    
    // setup kinect if using
    // @todo we only want to setup if connected
    // @note currently if you change the kinect setting you must restart
    int kinectFarCutOffPlane = 100; // 0 = far, 255 = near
    int kinectNearCutOffPlane = 225;
    int minContourSize = 10;
    kinectTracker.setup(kinectNearCutOffPlane, kinectFarCutOffPlane, minContourSize);
    
    // initialize our shape objects
    kinectShapeObject = new KinectShapeObject();
    kinectShapeObject->setKinectTracker(&kinectTracker);
    
    // set our current shape object to a default shape object
    currentShape = kinectShapeObject;
}

//--------------------------------------------------------------
void ReliefApplication::update(){
    
    // update the Kinect
    kinectTracker.update();
    
    // update the shape object
    currentShape->update();
}

//--------------------------------------------------------------
void ReliefApplication::draw(){
    ofBackground(100);
    
    // render the tangible display
    pinHeightMapImageSmall.begin();
    int w = pinHeightMapImageSmall.getWidth();
    int h = pinHeightMapImageSmall.getHeight();
    currentShape->renderTangibleShape(w, h);
    pinHeightMapImageSmall.end();
    
    // render the projector overlay image
    projectorOverlayImage.begin();
    w = projectorOverlayImage.getWidth();
    h = projectorOverlayImage.getHeight();
    currentShape->renderProjectorOverlay(w, h);
    projectorOverlayImage.end();
    
    // render the touch screen display
    touchScreenDisplayImage.begin();
    w = touchScreenDisplayImage.getWidth();
    h = touchScreenDisplayImage.getHeight();
    currentShape->renderTouchScreenGraphics(w, h);
    touchScreenDisplayImage.end();
    
    // render the vertical back display
    verticalDisplayImage.begin();
    w = verticalDisplayImage.getWidth();
    h = verticalDisplayImage.getHeight();
    currentShape->renderVerticalScreenGraphics(w, h);
    verticalDisplayImage.end();
    
    
    // draw our frame buffers
    pinHeightMapImageSmall.draw(  1,   1,   350, 350);
    projectorOverlayImage.draw(   1,   352, 350, 350);
    touchScreenDisplayImage.draw( 352, 1,   350, 350);
    verticalDisplayImage.draw(    352, 352, 350, 350);
    
    cameraTracker.drawCameraFeed("USB Camera", 351+352, 352, 350, 350);
    
    cameraTracker.drawCameraFeed("HD Pro Webcam C920",    351+352, 1,   350, 350);

}

//--------------------------------------------------------------
void ReliefApplication::keyPressed(int key){

}

//--------------------------------------------------------------
void ReliefApplication::keyReleased(int key){

}

//--------------------------------------------------------------
void ReliefApplication::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ReliefApplication::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ReliefApplication::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ReliefApplication::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ReliefApplication::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ReliefApplication::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ReliefApplication::dragEvent(ofDragInfo dragInfo){ 

}