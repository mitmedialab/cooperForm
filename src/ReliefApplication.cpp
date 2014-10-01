#include "ReliefApplication.h"
#include "UITriggers.h"

//--------------------------------------------------------------
void ReliefApplication::setup(){
    ofSetFrameRate(30);
    
    
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

    touchScreenDisplayImage.allocate(TOUCHSCREEN_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    verticalDisplayImage.allocate(VERTICAL_DISPLAY_SIZE_X, VERTICAL_DISPLAY_SIZE_Y, GL_RGBA);

    // setup camera interface
    cameraTracker.setup();
    
    
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
    
    
    // set our current shape object to a default shape object
//    setMode("telepresence");
    UITriggers::buttonTrigger(telepresenceModeButton);
    //currentShape = kinectShapeObject;
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
}

//--------------------------------------------------------------
void ReliefApplication::draw(){
    ofBackground(255);
    
    int w,h;
    
    // render the projector overlay image
    projectorOverlayImage.begin();
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
    
    ofRotate(90);
    ofTranslate(w, -h);
    currentShape->renderTangibleShape(-w, h);
    
    //ofTranslate(-w/2, -h/2);
    
    ofPopMatrix();
    
    pinHeightMapImage.end();
    
    
    ofPopStyle();
    
    
    /* render the touch screen display */
    touchScreenDisplayImage.begin();
    
    w = touchScreenDisplayImage.getWidth();
    h = touchScreenDisplayImage.getHeight();
    
    currentShape->renderTangibleShape(w, h);
    
    touchScreenDisplayImage.end();
    
    
    
    // render the vertical back display
    verticalDisplayImage.begin();
    
    w = verticalDisplayImage.getWidth();
    h = verticalDisplayImage.getHeight();
    currentShape->renderVerticalScreenGraphics(w, h);
    
    verticalDisplayImage.end();
    
    
    // draw our frame buffers
    //pinHeightMapImageSmall.draw(  1,   1,   350, 350);
    //projectorOverlayImage.draw(   1,   352, 350, 350);
    
    touchScreenDisplayImage.draw(420, 0, 1920 - 2*420, 1080);
   
    // draw UI stuff
    uiHandler.draw();
    
    //verticalDisplayImage.draw(    352, 352, 350, 350);
    
    // draw camera feeds
    w = 1920;
    h = 1080;
    
    cameraTracker.drawCameraFeed(0, w, 0, w, h);
    
//    pinHeightMapImage.draw(      200,   1,   350, 350);
//    pinHeightMapImageSmall.draw( 550,   1,   350, 350);
}

void ReliefApplication::exit(){
    
    update();
    draw();
    
    //mIOManager->disconnectFromTable();
    mIOManager->disconnectFromTableWithoutPinReset();
}


void ReliefApplication::setupUI() {
    uiHandler = UIHandler();
    
    
    // Right sidebar stuff:
    
    const int modeButtonWidth  = 372;
    const int modeButtonHeight = 222;
    
    const int sideBarRightSize = 420;
    
    const int modeButtonX = 1920 - sideBarRightSize + sideBarRightSize/2 - modeButtonWidth/2;
    const int modeButtonVertSpaceBetween = 50;
    const int modeButtonsTotalHeight = 4 * modeButtonHeight + 3 * modeButtonVertSpaceBetween;
    const int modeButtonStartY = 1080 / 2 - modeButtonsTotalHeight / 2;
    
    ofImage* divider = new ofImage();
    divider->loadImage("divider.png");
    
    const int dividerWidth = divider->getWidth();
    const int dividerHeight = divider->getHeight();
    const int dividerX = 1920 - sideBarRightSize + sideBarRightSize/2 - dividerWidth/2;
    
    
    // initialize the new buttons
    // UIButton name = UIButton("name", x,y, w,h)
    telepresenceModeButton
    = new UIButton("telepresence",
                   modeButtonX, modeButtonStartY,
                   modeButtonWidth,modeButtonHeight);
    telepresenceModeButton->setImageIdle("teleo.png");
    telepresenceModeButton->setImageActive("teleo-active.png");
    telepresenceModeButton->setImageSelected("teleo-selected.png");
    
    UIImage* divider1 = new UIImage(divider,
                                    dividerX, modeButtonStartY + modeButtonHeight + 0.5*modeButtonVertSpaceBetween - dividerHeight/2);
    
    wavyModeButton
    = new UIButton("wavy",
                   modeButtonX, modeButtonStartY + modeButtonVertSpaceBetween + modeButtonHeight,
                   modeButtonWidth,modeButtonHeight);
    wavyModeButton->setImageIdle("wave.png");
    wavyModeButton->setImageActive("wave-active.png");
    wavyModeButton->setImageSelected("wave-selected.png");
    UIImage* divider2 = new UIImage(divider,
                                    dividerX, modeButtonStartY + 2*modeButtonHeight + 1.5*modeButtonVertSpaceBetween - dividerHeight/2);
    
    
    threeDModeButton
    = new UIButton("3D",
                   modeButtonX, modeButtonStartY + 2*modeButtonVertSpaceBetween + 2*modeButtonHeight,
                   modeButtonWidth,modeButtonHeight);
    threeDModeButton->setImageIdle("3dm.png");
    threeDModeButton->setImageActive("3dm-active.png");
    threeDModeButton->setImageSelected("3dm-selected.png");
    UIImage* divider3 = new UIImage(divider,
                                    dividerX, modeButtonStartY + 3*modeButtonHeight + 2.5*modeButtonVertSpaceBetween - dividerHeight/2);
    
    
    mathModeButton
    = new UIButton("math",
                   modeButtonX, modeButtonStartY + 3*modeButtonVertSpaceBetween + 3*modeButtonHeight,
                   modeButtonWidth,modeButtonHeight);
    mathModeButton->setImageIdle("math.png");
    mathModeButton->setImageActive("math-active.png");
    mathModeButton->setImageSelected("math-selected.png");
    
    // add buttons to the handler
    uiHandler.addButton(telepresenceModeButton);
    uiHandler.addImage(divider1);
    uiHandler.addButton(wavyModeButton);
    uiHandler.addImage(divider2);
    uiHandler.addButton(threeDModeButton);
    uiHandler.addImage(divider3);
    uiHandler.addButton(mathModeButton);
    
    
    // left sidebar stuff:
    // inFORM logo
    const int sideBarLeftSize = 420;
    
    ofImage* inFORMLogoImg = new ofImage();
    inFORMLogoImg->loadImage("inform-logo.png");
    
    const int informLogoWidth = inFORMLogoImg->getWidth();
    const int informLogoX = sideBarLeftSize/2 - informLogoWidth/2;
    const int informLogoY = informLogoX;
    
    UIImage *inFORMLogo = new UIImage(inFORMLogoImg, informLogoX, informLogoY);
    
    // tmg logo
    ofImage* tmgLogImg = new ofImage();
    tmgLogImg->loadImage("tmg-logo.png");
    
    const int tmgLogoWidth = tmgLogImg->getWidth();
    const int tmgLogoHeight = tmgLogImg->getHeight();
    const int tmgLogoX = sideBarLeftSize/2 - tmgLogoWidth/2;
    const int tmgLogoY = 1080 - tmgLogoHeight - tmgLogoX;
    
    UIImage *tmgLogo = new UIImage(tmgLogImg, tmgLogoX, tmgLogoY);
    
    // test slider
    const int sliderWidth = 300;
    const int sliderX = sideBarLeftSize/2 - sliderWidth/2;
    const int sliderY = 1080/2;
    // name, bool horizontal, int trackX, int trackY, int trackLength, int handleWidth, int handleHeight
    UISlider *slider = new UISlider("slider", true, sliderX, sliderY, sliderWidth, 50, 50);
    
    
    uiHandler.addImage(inFORMLogo);
    uiHandler.addImage(tmgLogo);
    uiHandler.addSlider(slider);
    
    // drop shadow for middle region
    ofImage* centerDropShadowImg = new ofImage();
    centerDropShadowImg->loadImage("pin-shadow.png");
    
    const int dropShadowX = sideBarLeftSize + 1;
    const int dropShadowY = 1;
    
    UIImage *centerDropShadow = new UIImage(centerDropShadowImg, dropShadowX, dropShadowY);
    
    uiHandler.addImage(centerDropShadow);
    
    
    
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
    if (newMode == currentMode)
        return;
    
    if (newMode == "telepresence" || newMode == "wavy" || newMode == "3D" || newMode == "math")
        currentMode = newMode;
    else
        cout << "Invalid mode selected";
    
    if (currentMode == "telepresence")
        currentShape = kinectShapeObject;
    else if (currentMode == "wavy") {
        wavyShapeObject->reset();
        currentShape = wavyShapeObject;
    }
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