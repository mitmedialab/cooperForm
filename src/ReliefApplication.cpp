#include "ReliefApplication.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    // initialize the UI
    setupUI();
    
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
    wavyShapeObject   = new WavyShapeObject(24*2,24*2);
    wavyShapeObject->setKinectTracker(&kinectTracker);
    
    
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
    
    
    /* render the touch screen display */
    //touchScreenDisplayImage.begin();
    
    w = touchScreenDisplayImage.getWidth()-200;
    h = touchScreenDisplayImage.getHeight();
    
    ofPushMatrix();
    ofTranslate(200, 0);
    currentShape->renderTangibleShape(w, h);
    ofPopMatrix();
    
    // draw UI stuff
    uiHandler.draw();
    
    
    //touchScreenDisplayImage.end();
    
    
    
    // render the vertical back display
    verticalDisplayImage.begin();
    w = verticalDisplayImage.getWidth();
    h = verticalDisplayImage.getHeight();
    currentShape->renderVerticalScreenGraphics(w, h);
    verticalDisplayImage.end();
    
    
    // draw our frame buffers
    //pinHeightMapImageSmall.draw(  1,   1,   350, 350);
    //projectorOverlayImage.draw(   1,   352, 350, 350);
    //touchScreenDisplayImage.draw(0, 0, ofGetWidth()/2, ofGetHeight());
    //verticalDisplayImage.draw(    352, 352, 350, 350);
    
    // draw camera feeds
    w = ofGetWidth() * 0.5;
    h = ofGetHeight();
    
    cameraTracker.drawCameraFeed(0, w+1, 1,   w, h);
    //cameraTracker.drawCameraFeed(1, 2*(w+1), 1,   w, h);
    
}


void ReliefApplication::setupUI() {
    uiHandler = UIHandler();
    
    const int modeButtonWidth  = 200;
    const int modeButtonHeight = 150;
    
    // initialize the new buttons
    // UIButton name = UIButton("name", x,y, w,h)
    telepresenceModeButton
    = new UIButton("telepresence", 0,0,                      modeButtonWidth,modeButtonHeight);
    //telepresenceModeButton->setImage("tele.png");
    
    wavyModeButton
    = new UIButton("wavy",         0,  (modeButtonHeight+1), modeButtonWidth,modeButtonHeight);
    //wavyModeButton->setImage("waves.png");
    
    threeDModeButton
    = new UIButton("3D",           0,2*(modeButtonHeight+1), modeButtonWidth,modeButtonHeight);
    //threeDModeButton->setImage("3d.png");
    
    mathModeButton
    = new UIButton("math",         0,3*(modeButtonHeight+1), modeButtonWidth,modeButtonHeight);
    //mathModeButton->setImage("math.png");
    
    // add buttons to the handler
    uiHandler.addButton(telepresenceModeButton);
    uiHandler.addButton(wavyModeButton);
    uiHandler.addButton(threeDModeButton);
    uiHandler.addButton(mathModeButton);
    
    // add these buttons to a group
    // for the "select" and "unselect" functionality
    vector<UIButton*> mainButtonGroup = vector<UIButton*>();
    mainButtonGroup.push_back(telepresenceModeButton);
    mainButtonGroup.push_back(wavyModeButton);
    mainButtonGroup.push_back(threeDModeButton);
    mainButtonGroup.push_back(mathModeButton);
    uiHandler.addButtonGroup(mainButtonGroup);
    
}

// change the relief application mode

void ReliefApplication::setMode(string newMode) {
    if (newMode == "telepresence" || newMode == "wavy" || newMode == "3D" || newMode == "math")
        currentMode = newMode;
    else
        cout << "Invalid mode selected";
    
    if (currentMode == "telepresence")
        currentShape = kinectShapeObject;
    else if (currentMode == "wavy")
        currentShape = wavyShapeObject;
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
    uiHandler.mouseDragged(x,y);
}

//--------------------------------------------------------------
void ReliefApplication::mousePressed(int x, int y, int button){
    uiHandler.mousePressed(x,y);
}

//--------------------------------------------------------------
void ReliefApplication::mouseReleased(int x, int y, int button){
    uiHandler.mouseReleased(x,y);
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