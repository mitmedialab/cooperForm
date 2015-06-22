#include "ReliefApplication.h"
#include "UITriggers.h"
#include "GUI.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    CGDisplayHideCursor(NULL);
  
    // initialize the UI
    setupUI();
    
    // set up OSCInterface
    // needs to be setup before UI
    backDisplayComputer = new OSCInterface(BACK_COMPUTER_IP, BACK_COMPUTER_PORT);
    
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
    const int kinectFarCutOffPlane = KINECT_FAR_CUTOFF_PLANE; // 0 = far, 255 = near
    const int kinectNearCutOffPlane = KINECT_NEAR_CUTOFF_PLANE; // 0 = far, 255 = near
    const int minContourSize = KINECT_CONTOUR_SIZE;
    kinectTracker.setup(kinectNearCutOffPlane, kinectFarCutOffPlane, minContourSize);
    const int kinectCropWidth  = KINECT_CROP_WIDTH;
    const int kinectCropHeight = KINECT_CROP_HEIGHT;
    const int kinectCropX = KINECT_CROP_X;
    const int kinectCropY = KINECT_CROP_Y;
    kinectTracker.setCrop(kinectCropX, kinectCropY, kinectCropWidth, kinectCropHeight);
    
    
    // initialize our shape objects
    touchShapeObject = new TouchShapeObject();
    touchShapeObject->setPinHeightsFromDisplayContainer(pinHeightsFromDisplay);
    kinectShapeObject = new KinectShapeObject();
    kinectShapeObject->setKinectTracker(&kinectTracker);
    kinectShapeObject->setCameraTracker(&cameraTracker);
    wavyShapeObject   = new WavyShapeObject(24,24);
    wavyShapeObject->setKinectTracker(&kinectTracker);
    
    threeDShapeObject = new ThreeDShapeObject();
    cityShapeObject = new CityShapeObject();
    
    mathShapeObject = new MathShapeObject();
    
    ballMoverShapeObject = new MoveBallShapeObject();
    overlayShape = ballMoverShapeObject;
    
    transitionLengthMS = 500; // milliseconds of transition between shape objects
   
    
    // set our current shape object to a default shape object
    UITriggers::buttonTrigger(uiHandler->getButton("telepresence"));
    
    // and default math equation
    UITriggers::buttonTrigger(uiHandler->getButton("equationSelectButton1"));
    
    // and default city scape mode
    UITriggers::buttonTrigger(uiHandler->getButton("citySelectButton1"));
    
    currentTransitionFromShape = currentShape;
    currentTransitionToShape = currentShape;
}

//--------------------------------------------------------------
void ReliefApplication::update(){
    // get the current pin heights in the display
    for (int x = 0; x < RELIEF_SIZE_X; x++) {
        for (int y = 0; y < RELIEF_SIZE_Y; y++) {
            pinHeightsFromDisplay[x][y] = mIOManager->pinHeightFromRelief[x][y];
        }
    }
    
    // update the Kinect
    kinectTracker.update();
    
    // switch to idle if no activity after x seconds on the depth camera
    if (kinectTracker.timeSinceLastActive() > KINECT_ACTIVITY_TIMEOUT_SEC && currentMode != "idle") {
        setMode("idle");
        
    } else if (kinectTracker.timeSinceLastActive() <= KINECT_ACTIVITY_TIMEOUT_SEC && currentMode == "idle") {
        setMode("telepresence");
        UITriggers::buttonTrigger(uiHandler->getButton("telepresence"));
    }
    
    // update the necessary shape objects
    if (ofGetElapsedTimeMillis() - transitionStart > transitionLengthMS) {
        currentTransitionFromShape->update();
        currentTransitionToShape->update();
    } else {
        currentShape->update();
    }
    
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
    //UITriggers::sliderTrigger(uiHandler->getSlider("sliderPosition"));
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
    //else if (currentMode == "city") {
    //    overlayShape->renderTangibleShape(w, h);
    //    currentShape->renderTangibleShape(w, h);
    //} else {
        overlayShape->renderTangibleShape(-w, h);
        currentShape->renderTangibleShape(-w, h);
    //}
    
    overlayShape->renderTangibleShape(-w, h);
    
    ofPopMatrix();
    pinHeightMapImage.end();
    ofPopStyle();
    
    // render the touch screen display
    touchScreenDisplayImage.begin();
    
    w = touchScreenDisplayImage.getWidth();
    h = touchScreenDisplayImage.getHeight();
    
    ofBackground(255); //refresh
    
    currentShape->renderTouchscreenGraphics(w, h);
    
    touchScreenDisplayImage.end();
    
    // draw our frame buffers
    if (currentMode == "city") {
        touchScreenDisplayImage.draw(MARGIN_X , 0, w, h);
    } else {
        touchScreenDisplayImage.draw(MARGIN_X + w, 0, -w, h);
    }

    // draw margin image
    if (currentMode == "3D") {
        currentShape->renderMarginGraphics(0, 460);
        uiHandler->getText("modelName")->setText(threeDShapeObject->getCurrentModelName());
    }
    if (currentMode == "math") {
        //cout<<mathShapeObject->getEqVal1()<<endl;
        //cout<<uiHandler->getNum("eqVal1")->getName() <<endl;
        UIImage* equation = uiHandler->getImage("equation");
        ofImage* eqImg = mathShapeObject->getEqImage();
        
        const int eqImageWidth = eqImg->getWidth();
        const int eqImageX = TOUCHSCREEN_SIZE_X/2 - eqImageWidth/2;
        const int eqNumY = equation->getY() + 60;
        equation->setImage(eqImg);
        equation->setX(eqImageX);
        
        vector<OffsetAndFont> firstVarXOffsets  = mathShapeObject->getVal1XOffsets();
        vector<OffsetAndFont> secondVarXOffsets = mathShapeObject->getVal2XOffsets();
        
        UINum* eqVal1 = uiHandler->getNum("eqVal1");
        UINum* eqVal2 = uiHandler->getNum("eqVal2");
        
        eqVal1->setX( firstVarXOffsets[0].offsetX + eqImageX);
        eqVal2->setX(secondVarXOffsets[0].offsetX + eqImageX);
        
        eqVal1->setY( firstVarXOffsets[0].offsetY + eqNumY);
        eqVal2->setY(secondVarXOffsets[0].offsetY + eqNumY);
        
        eqVal1->setSize( firstVarXOffsets[0].fontSizeOffset + 26);
        eqVal2->setSize(secondVarXOffsets[0].fontSizeOffset + 26);
        
        eqVal1->setNum(mathShapeObject->getEqVal1());
        eqVal2->setNum(mathShapeObject->getEqVal2());
        
        
        UINum* eqValSec1 = uiHandler->getNum("eqValSecond1");
        UINum* eqValSec2 = uiHandler->getNum("eqValSecond2");
        
        if (firstVarXOffsets.size() > 1 && secondVarXOffsets.size() > 1) {
            eqValSec1->setX( firstVarXOffsets[1].offsetX + eqImageX);
            eqValSec2->setX(secondVarXOffsets[1].offsetX + eqImageX);
            
            eqValSec1->setY( firstVarXOffsets[1].offsetY + eqNumY);
            eqValSec2->setY(secondVarXOffsets[1].offsetY + eqNumY);
            
            eqValSec1->setSize( firstVarXOffsets[1].fontSizeOffset + 26);
            eqValSec2->setSize(secondVarXOffsets[1].fontSizeOffset + 26);
            
            eqValSec1->setNum(mathShapeObject->getEqVal1());
            eqValSec2->setNum(mathShapeObject->getEqVal2());
            
            if (eqVal1->showing()) {
                eqValSec1->show();
                eqValSec2->show();
            }
        }
        else {
            eqValSec1->hide();
            eqValSec2->hide();
        }
            
        
        uiHandler->getButton("modifyVal1Up")->setX(firstVarXOffsets[0].offsetX    + eqImageX);
        uiHandler->getButton("modifyVal1Down")->setX(firstVarXOffsets[0].offsetX  + eqImageX);
        uiHandler->getButton("modifyVal2Up")->setX(secondVarXOffsets[0].offsetX   + eqImageX);
        uiHandler->getButton("modifyVal2Down")->setX(secondVarXOffsets[0].offsetX + eqImageX);
    }
    if (currentMode == "city") {
        //cout<<mathShapeObject->getEqVal1()<<endl;
        //cout<<uiHandler->getNum("eqVal1")->getName() <<endl;
        
        
        // uncomment the code below for the UI slider.
        /*float timeFloat = cityShapeObject->getMovPosition(); //0.0to1.0
        int hour = floor(timeFloat*12);
        int minute = int(ofMap(timeFloat*12 - hour,0,1.0,0,60));
        string ampm;
        
        if (hour < 6 ){
            ampm = "am";
            hour += 6;
        } else {
            ampm = "pm";
            hour -= 6;
        }
        
        string timeStr;
        string hour0;
        string minute0;
        if(hour < 10){
            hour0 = "0";
        }
        if(minute<10) {
            minute0 = "0";
        }
        timeStr += hour0 + ofToString(hour)+":"+ minute0 +ofToString(minute) + ampm;
        
        uiHandler->getText("time")->setText(timeStr);
        uiHandler->getText("time")->setX(508+timeFloat*(905)-27);
        
        uiHandler->getImage("timeTip")->setX(508+timeFloat*(905) - 42);
        
        
        cityShapeObject->sliderGrabbed = uiHandler->getSlider("sliderPosition")->isGrabbed();
        if(cityShapeObject->sliderGrabbed == false){
            int val = (int)(uiHandler->getSlider("sliderPosition")->getVal());
            uiHandler->getSlider("sliderPosition")->setHandlePos(cityShapeObject->getMovPosition());//((val + 1) % 1000)/1000.0);//
            
        }*/
        // cout <<uiHandler->getSlider("sliderPosition")->getVal() << endl;
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
    
    //kinectTracker.drawActivityDepthImage(0, 0, 640, 480);
    //drawDebugScreen();
}

//--------------------------------------------------------------

void ReliefApplication::exit(){
    
    update();
    //draw();
    
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

void ReliefApplication::drawDebugScreen() {
    
    ofFill();
    // draw the table depth map:
    for (int x = 0; x < 24; x++){
        for (int y = 0; y < 24; y++) {
            int toHeight = mIOManager->pinHeightToRelief[x][y];
            int fromHeight = mIOManager->pinHeightFromRelief[x][y];
            int difference = mIOManager->pinDiscrepancy[x][y];
            int enabledColor = (mIOManager->pinEnabled[x][y])? 0 : 255;
            
            ofSetColor(toHeight);
            ofRect(x*5,y*5,5,5);
            
            ofSetColor(fromHeight);
            ofRect(x*5+125,y*5,5,5);
            
            ofSetColor(difference);
            ofRect(x*5+250,y*5,5,5);
            
            ofSetColor(enabledColor, 0, 0);
            ofRect(x*5+375,y*5,5,5);
        }
    }
}

// change the relief application mode

void ReliefApplication::setMode(string newMode) {
    if (newMode == currentMode)
        return;

    if (newMode == "idle") {
        UITriggers::buttonTrigger(uiHandler->getButton("telepresence"));
        UITriggers::setIdle();
        currentMode = newMode;
        backDisplayComputer->sendModeChange(newMode);
        
        currentTransitionFromShape = currentShape;
        currentShape = kinectShapeObject;
        currentTransitionToShape = currentShape;
        transitionStart = ofGetElapsedTimeMillis();
        
        //if (ballMoverShapeObject->isBallInCorner())
        //    ballMoverShapeObject->moveBallToCenter();
        
        mIOManager->set_max_speed(0);
        return;
    }
    
    mIOManager->set_max_speed(maxSpeed);
    
    if (newMode == "telepresence" || newMode == "wavy" || newMode == "city" || newMode == "3D" || newMode == "math" || newMode == "replicator" || newMode == "idle") {
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
        
        
        if (ballMoverShapeObject->isBallInCorner())
            ballMoverShapeObject->moveBallToCenter();
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
    else if (currentMode == "city") {
        cityShapeObject->reset();
        //uiHandler->getSlider("sliderScale")->setHandlePos(0.5);
        currentTransitionFromShape = currentShape;
        currentShape = cityShapeObject;
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

    else if (currentMode == "replicator") {
        currentTransitionFromShape = currentShape;
        currentShape = touchShapeObject;
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
            setMode("city");
            UITriggers::buttonTrigger(uiHandler->getButton("city"));
            break;
        case '4':
            setMode("math");
            UITriggers::buttonTrigger(uiHandler->getButton("math"));
            break;
        case '5':
            setMode("replicator");
            break;
        case '6':
            setMode("idle");
            break;
        case '0':
            mIOManager->set_max_speed(0);
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
    if(currentMode == "city") cityShapeObject->setMouseDragInfo(x, y, button);
}

//--------------------------------------------------------------
void ReliefApplication::mousePressed(int x, int y, int button){
    uiHandler->mousePressed(x,y);
    if(currentMode == "3D") threeDShapeObject->setMousePressedInfo(x, y);
    if(currentMode == "city") cityShapeObject->setMousePressedInfo(x, y);
}

//--------------------------------------------------------------
void ReliefApplication::mouseReleased(int x, int y, int button){
    uiHandler->mouseReleased(x,y);
    if(currentMode == "city") cityShapeObject->setMouseReleasedInfo();
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