#include "ReliefApplication.h"
#include "UITriggers.h"
#include "GUI.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
  
    // initialize the UI
    setupUI();
    
    // set up OSCInterface
    // needs to be setup before UI
    backDisplayComputer = new OSCInterface("168.176.175.2", 4444);
    
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
    projectorOverlayImage.allocate(PROJECTOR_SIZE_X, PROJECTOR_SIZE_Y, GL_RGB);
    
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y, GL_RGB);
    transitionImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y, GL_RGB);
    
    pinHeightMapImageSmall.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y, GL_RGBA);
    pinHeightMapImageSmall.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

    touchScreenDisplayImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    marginTouchDisplayImage.allocate(MARGIN_X, MARGIN_X, GL_RGBA);

    // setup camera interface
    cameraTracker.setup();
    
    
    // setup kinect if using
    // @todo we only want to setup if connected
    // @note currently if you change the kinect setting you must restart
    const int kinectFarCutOffPlane = 218; // 225 // 0 = far, 255 = near
    const int kinectNearCutOffPlane = 235; // 235
    const int minContourSize = 10;
    kinectTracker.setup(kinectNearCutOffPlane, kinectFarCutOffPlane, minContourSize);
    const int kinectCropWidth  = 240;
    const int kinectCropHeight = 240;
    const int kinectCropX = 640 / 2 - kinectCropWidth/2; // centered crop x
    const int kinectCropY = 480 / 2 - kinectCropHeight/2 - 40; // centered crop y offset
    kinectTracker.setCrop(kinectCropX, kinectCropY, kinectCropWidth, kinectCropHeight);
    
    
    // initialize our shape objects
    kinectShapeObject = new KinectShapeObject();
    kinectShapeObject->setKinectTracker(&kinectTracker);
    kinectShapeObject->setCameraTracker(&cameraTracker);
    wavyShapeObject   = new WavyShapeObject(24,24);
    wavyShapeObject->setKinectTracker(&kinectTracker);
    
    threeDShapeObject = new ThreeDShapeObject();
    mathShapeObject = new MathShapeObject();
    
    ballMoverShapeObject = new MoveBallShapeObject();
    overlayShape = ballMoverShapeObject;
    
    transitionLengthMS = 500; // milliseconds of transition between shape objects
   
    
    // set our current shape object to a default shape object
    UITriggers::buttonTrigger(uiHandler->getButton("telepresence"));
    
    currentTransitionFromShape = currentShape;
    currentTransitionToShape = currentShape;
}

//--------------------------------------------------------------
void ReliefApplication::update(){
    
    // update the Kinect
    kinectTracker.update();
    
    // update the necessary shape objects
    if (ofGetElapsedTimeMillis() - transitionStart > transitionLengthMS) {
        currentTransitionFromShape->update();
        currentTransitionToShape->update();
    }
    else
        currentShape->update();
    overlayShape->update();
    
    
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
    long timeMS = ofGetElapsedTimeMillis();
    if (timeMS - transitionStart < transitionLengthMS) {
        
        // draw semi transparent transitionImage
        transitionImage.begin();
        ofBackground(0);
        currentTransitionFromShape->renderTangibleShape(w, h);
        transitionImage.end();
        
        currentTransitionToShape->renderTangibleShape(-w, h);
        
        // overlay the transition image with transparency
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(255, 255 - 255 * (double)(timeMS - transitionStart) / transitionLengthMS);
        transitionImage.draw(0,0, -w,h);
        ofDisableAlphaBlending();
        ofPopStyle();
    }
    else
        currentShape->renderTangibleShape(-w, h);
    
    overlayShape->renderTangibleShape(-w, h);
    
    ofPopMatrix();
    pinHeightMapImage.end();
    ofPopStyle();
    
    // render the touch screen display
    touchScreenDisplayImage.begin();
    
    w = touchScreenDisplayImage.getWidth();
    h = touchScreenDisplayImage.getHeight();
    
    ofBackground(255); //refresh
    
    //ofPushStyle();
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
    
    //cout << "mouse " << ofGetMouseX()/4 << " " << ofGetMouseY()/4 << endl;
    //ofSetColor(ofColor::fromHsb(ofGetMouseX()/4, ofGetMouseY()/4, 255)); // c is bright saturated cyan);
    //cameraTracker.drawCameraFeed(0, -194, -26, 1502, 1120);
    //glDisable(GL_BLEND);
    //ofPopStyle();
    
    //cameraTracker.drawCameraFeed(0, 0, 0, w, h);
    //currentShape->renderTangibleShape(w, h);
    currentShape->renderTouchscreenGraphics(w, h);
    touchScreenDisplayImage.end();
    
    
    // draw our frame buffers
    touchScreenDisplayImage.draw(MARGIN_X + w, 0, -w, h);
    
    
    // draw margin image
    if (currentMode == "3D") {
        currentShape->renderMarginGraphics(0, 460);
        uiHandler->getText("modelName")->setText(threeDShapeObject->getCurrentModelName());
    }
    
    if (currentMode == "math") {
        //cout<<mathShapeObject->getEqVal1()<<endl;
        //cout<<uiHandler->getNum("eqVal1")->getName() <<endl;
        string equationStr = mathShapeObject->getEq();
        UIText * equationText = uiHandler->getText("equation");
        
        int firstVar = equationStr.find_first_of('_');
        int firstVarX = equationText->getStrWidth(equationStr.substr(0, firstVar));
        equationStr = equationStr.replace(firstVar, 1, " ");
        
        int secondVar = equationStr.find_first_of('_');
        int secondVarX = equationText->getStrWidth(equationStr.substr(0, secondVar));
        equationStr = equationStr.replace(secondVar, 1, " ");
        
        while (equationStr.find('_') != std::string::npos) {
            int var1 = equationStr.find_first_of('_');
            equationStr = equationStr.replace(var1, 1, mathShapeObject->getEqVal1());
            
            int var2 = equationStr.find_first_of('_');
            equationStr = equationStr.replace(var2, 1, mathShapeObject->getEqVal2());
        }
        int equationWidth = equationText->getStrWidth(equationStr);
        uiHandler->getNum("eqVal1")->setX(firstVarX - equationWidth/2 + equationText->getX() - 30);
        uiHandler->getNum("eqVal2")->setX(secondVarX - equationWidth/2 + equationText->getX() - 30);
        
        
        equationText->setText(equationStr);
        uiHandler->getNum("eqVal1")->setNum(mathShapeObject->getEqVal1());
        uiHandler->getNum("eqVal2")->setNum(mathShapeObject->getEqVal2());
        
    }

    // draw UI stuff
    uiHandler->draw();
    
    // draw the projector image
    
    w = projectorOverlayImage.getWidth();
    h = projectorOverlayImage.getHeight();
    
    ofPushStyle();
    ofSetColor(0);
    ofRect(1920, 0, 1920, 1080);
    ofPopStyle();
    //cameraTracker.drawCameraFeed(0, w, 0, w, h);
    projectorOverlayImage.draw(1920, 0, w, h);
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
    
    if (currentMode == "telepresence") {
        currentTransitionFromShape = currentShape;
        currentShape = kinectShapeObject;
        currentTransitionToShape = currentShape;
        transitionStart = ofGetElapsedTimeMillis();
        
        if (ballMoverShapeObject->isBallInCorner())
            ballMoverShapeObject->moveBallToCenter();
    }
    else if (currentMode == "wavy") {
        wavyShapeObject->reset();
        currentTransitionFromShape = currentShape;
        currentShape = wavyShapeObject;
        currentTransitionToShape = currentShape;
        transitionStart = ofGetElapsedTimeMillis();
        
        
        if (!ballMoverShapeObject->isBallInCorner())
            ballMoverShapeObject->moveBallToCorner();
    }
    else if (currentMode == "3D") {
        threeDShapeObject->reset();
        uiHandler->getSlider("sliderScale")->setHandlePos(0.5);
        currentTransitionFromShape = currentShape;
        currentShape = threeDShapeObject;
        currentTransitionToShape = currentShape;
        transitionStart = ofGetElapsedTimeMillis();
        
        if (!ballMoverShapeObject->isBallInCorner())
            ballMoverShapeObject->moveBallToCorner();
    }
    else if (currentMode == "math") {
        mathShapeObject->reset();
        currentTransitionFromShape = currentShape;
        currentShape = mathShapeObject;
        currentTransitionToShape = currentShape;
        transitionStart = ofGetElapsedTimeMillis();
        
        if (!ballMoverShapeObject->isBallInCorner())
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
    if(key == ' ')
        if(currentMode == "math") mathShapeObject->nextFunction();
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