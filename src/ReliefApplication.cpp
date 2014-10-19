#include "ReliefApplication.h"
#include "UITriggers.h"
#include "GUI.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
  
    
    // set up OSCInterface
    // needs to be setup before UI
    backDisplayComputer = new OSCInterface("169.169.169.169", 86753);
    
    // initialize the UI
    setupUI();
    
    // initialize communication with the pin display
	mIOManager = new ShapeIOManager();
    mIOManager->connectToTable();
    
    // setup default valus for pins
    // @todo move to config file?
    gain_P = 1.5;
    gain_I = 0.045;
    max_I = 60;
    deadZone = 2;
    maxSpeed = 220;
    
    mIOManager->set_gain_p(gain_P);
    mIOManager->set_gain_i(gain_I);
    mIOManager->set_max_i(max_I);
    mIOManager->set_deadzone(deadZone);
    mIOManager->set_max_speed(maxSpeed);
    
    // allocate all the necessary frame buffer objects
    projectorOverlayImage.allocate(1920, 1080, GL_RGB);
    
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y, GL_RGB);
    
    pinHeightMapImageSmall.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y, GL_RGBA);
    pinHeightMapImageSmall.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

    touchScreenDisplayImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    marginTouchDisplayImage.allocate(MARGIN_X, MARGIN_X, GL_RGBA);

    // setup camera interface
    //cameraTracker.setup();
    
    
    // setup kinect if using
    // @todo we only want to setup if connected
    // @note currently if you change the kinect setting you must restart
    int kinectFarCutOffPlane = 225; // 0 = far, 255 = near
    int kinectNearCutOffPlane = 235;
    int minContourSize = 10;
    kinectTracker.setup(kinectNearCutOffPlane, kinectFarCutOffPlane, minContourSize);
    kinectTracker.setCrop(320-120, 240-120, 240, 240);
    
    // initialize our shape objects
    kinectShapeObject = new KinectShapeObject();
    kinectShapeObject->setKinectTracker(&kinectTracker);
    wavyShapeObject   = new WavyShapeObject(24,24);
    wavyShapeObject->setKinectTracker(&kinectTracker);
    
    threeDShapeObject = new ThreeDShapeObject();
    
    ballMoverShapeObject = new MoveBallShapeObject();
    
    
    // set our current shape object to a default shape object
    UITriggers::buttonTrigger(uiHandler->getButton("telepresence"));
}

//--------------------------------------------------------------
void ReliefApplication::update(){
    
    // update the Kinect
    kinectTracker.update();
    
    // update the shape object
    currentShape->update();
    
    
    // draw the big heightmap image into a small heightmap image and send it off to the table
    pinHeightMapImageSmall.begin();
    
    ofBackground(0);
    ofSetColor(255);
    pinHeightMapImage.draw(0, 0, RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
    
    pinHeightMapImageSmall.end();
    
    
    // send height map image to the tangible display
    mIOManager->update(pinHeightMapImageSmall);
    
    // trigger slider to init value
    UITriggers::sliderTrigger(uiHandler->getSlider("sliderScale"));
}

//--------------------------------------------------------------
void ReliefApplication::draw(){
    ofBackground(255);
    
    int w,h;
    
    // render the projector overlay image
    projectorOverlayImage.begin();
    ofBackground(0);
    w = projectorOverlayImage.getWidth();
    h = projectorOverlayImage.getHeight();
    currentShape->renderProjectorOverlay(w, h);
    projectorOverlayImage.end();
    
    
    // render the tangible display
    ofPushStyle();
    
    pinHeightMapImage.begin();
    ofBackground(0);
    ofSetColor(ofColor(200));
    
    w = pinHeightMapImage.getWidth();
    h = pinHeightMapImage.getHeight();
    
    ofPushMatrix();
    // do some transformations so the tangible display is at the right orientation
    // (mirroring the user)
    ofRotate(90);
    ofTranslate(w, -h);
    currentShape->renderTangibleShape(-w, h);
    
    if (ballMoverShapeObject->isBallInCorner()) {
        ofSetColor(255);
        ofCircle(10,10, 10,10);
        ofSetColor(0);
        ofCircle(10,10, 6,6);
    }
    
    ofPopMatrix();
    pinHeightMapImage.end();
    
    ofPopStyle();
    
    // render the touch screen display
    touchScreenDisplayImage.begin();
    
    w = touchScreenDisplayImage.getWidth();
    h = touchScreenDisplayImage.getHeight();
    
    ofBackground(255); //refresh
    //currentShape->renderTangibleShape(w, h);
    currentShape->renderTouchscreenGraphics(w, h);
    touchScreenDisplayImage.end();
    
    
    // draw our frame buffers
    touchScreenDisplayImage.draw(MARGIN_X, 0);
    
    
    // draw margin image
    if (currentMode == "3D") {
        currentShape->renderMarginGraphics(0, 0);
    }

    // draw UI stuff
    uiHandler->draw();
    
    // draw the projector image
    w = 1920;
    h = 1080;
    projectorOverlayImage.draw(w, 0, w, h);
}

//--------------------------------------------------------------

void ReliefApplication::exit(){
    
    update();
    draw();
    
    //mIOManager->disconnectFromTable();
    mIOManager->disconnectFromTableWithoutPinReset();
}

//--------------------------------------------------------------

void ReliefApplication::setupUI() {
    uiHandler = new UIHandler();
    UITriggers::registerReliefApplication(this);
    GUI::setupUI(uiHandler);
}

//--------------------------------------------------------------

// change the relief application mode

void ReliefApplication::setMode(string newMode) {
    if (newMode == currentMode)
        return;
    
    if (newMode == "telepresence" || newMode == "wavy" || newMode == "3D" || newMode == "math") {
        currentMode = newMode;
        backDisplayComputer->sendModeChange(newMode);
    }
    else
        cout << "Invalid mode selected" << endl;
    
    if (currentMode == "telepresence")
        currentShape = kinectShapeObject;
    else if (currentMode == "wavy") {
        wavyShapeObject->reset();
        currentShape = wavyShapeObject;
    }
    else if (currentMode == "3D") {
        threeDShapeObject->reset();
        currentShape = threeDShapeObject;
    }
    else if (currentMode == "math") {
        currentShape = ballMoverShapeObject;
        ballMoverShapeObject->moveBallToCorner();
    }

}

//--------------------------------------------------------------
void ReliefApplication::keyPressed(int key){
    switch(key)
    {
        case '1':
            setMode("telepresence");
            UITriggers::buttonTrigger(uiHandler->getButton("telepresence"));
            break;
        case '2':
            setMode("wavy");
            UITriggers::buttonTrigger(uiHandler->getButton("wavy"));
            break;
        case '3':
            setMode("3D");
            UITriggers::buttonTrigger(uiHandler->getButton("3D"));
            break;
        case '4':
            setMode("math");
            UITriggers::buttonTrigger(uiHandler->getButton("math"));
            break;
    }
}

//--------------------------------------------------------------
void ReliefApplication::keyReleased(int key){

}

//--------------------------------------------------------------
void ReliefApplication::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ReliefApplication::mouseDragged(int x, int y, int button){
    uiHandler->mouseDragged(x,y);
    if(currentMode == "3D") threeDShapeObject->setMouseDragInfo(x, y, button);
}

//--------------------------------------------------------------
void ReliefApplication::mousePressed(int x, int y, int button){
    uiHandler->mousePressed(x,y);
    if(currentMode == "3D") threeDShapeObject->setMousePressedInfo(x, y);
}

//--------------------------------------------------------------
void ReliefApplication::mouseReleased(int x, int y, int button){
    uiHandler->mouseReleased(x,y);
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