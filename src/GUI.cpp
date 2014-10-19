//
//  GUI.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 10/10/14.
//
//

// Constructs the GUI

#include "GUI.h"
void GUI::setupRightSidebar(UIHandler *uiHandler, const int rightSidebarWidth) {
    // Right sidebar stuff:
    
    const int modeButtonWidth  = 372;
    const int modeButtonHeight = 222;
    
    const int modeButtonX = 1920 - rightSidebarWidth + rightSidebarWidth/2 - modeButtonWidth/2;
    const int modeButtonVertSpaceBetween = 50;
    const int modeButtonsTotalHeight = 4 * modeButtonHeight + 3 * modeButtonVertSpaceBetween;
    const int modeButtonStartY = 1080 / 2 - modeButtonsTotalHeight / 2;
    
    ofImage* divider = new ofImage();
    divider->loadImage("divider.png");
    
    const int dividerWidth = divider->getWidth();
    const int dividerHeight = divider->getHeight();
    const int dividerX = 1920 - rightSidebarWidth + rightSidebarWidth/2 - dividerWidth/2;
    
    
    // initialize the new buttons
    // UIButton name = UIButton("name", x,y, w,h)
    UIButton* telepresenceModeButton
    = new UIButton("telepresence",
                   modeButtonX, modeButtonStartY,
                   modeButtonWidth,modeButtonHeight);
    telepresenceModeButton->setImageIdle("teleo.png");
    telepresenceModeButton->setImageActive("teleo-active.png");
    telepresenceModeButton->setImageSelected("teleo-selected.png");
    
    UIImage* divider1 = new UIImage(divider,
                                    dividerX, modeButtonStartY + modeButtonHeight + 0.5*modeButtonVertSpaceBetween - dividerHeight/2);
    
    UIButton* wavyModeButton
    = new UIButton("wavy",
                   modeButtonX, modeButtonStartY + modeButtonVertSpaceBetween + modeButtonHeight,
                   modeButtonWidth,modeButtonHeight);
    wavyModeButton->setImageIdle("wave.png");
    wavyModeButton->setImageActive("wave-active.png");
    wavyModeButton->setImageSelected("wave-selected.png");
    UIImage* divider2 = new UIImage(divider,
                                    dividerX, modeButtonStartY + 2*modeButtonHeight + 1.5*modeButtonVertSpaceBetween - dividerHeight/2);
    
    
    UIButton* threeDModeButton
    = new UIButton("3D",
                   modeButtonX, modeButtonStartY + 2*modeButtonVertSpaceBetween + 2*modeButtonHeight,
                   modeButtonWidth,modeButtonHeight);
    threeDModeButton->setImageIdle("3dm.png");
    threeDModeButton->setImageActive("3dm-active.png");
    threeDModeButton->setImageSelected("3dm-selected.png");
    UIImage* divider3 = new UIImage(divider,
                                    dividerX, modeButtonStartY + 3*modeButtonHeight + 2.5*modeButtonVertSpaceBetween - dividerHeight/2);
    
    
    UIButton* mathModeButton
    = new UIButton("math",
                   modeButtonX, modeButtonStartY + 3*modeButtonVertSpaceBetween + 3*modeButtonHeight,
                   modeButtonWidth,modeButtonHeight);
    mathModeButton->setImageIdle("math.png");
    mathModeButton->setImageActive("math-active.png");
    mathModeButton->setImageSelected("math-selected.png");
    
    // add buttons to the handler
    uiHandler->addButton(telepresenceModeButton);
    uiHandler->addImage(divider1);
    uiHandler->addButton(wavyModeButton);
    uiHandler->addImage(divider2);
    uiHandler->addButton(threeDModeButton);
    uiHandler->addImage(divider3);
    uiHandler->addButton(mathModeButton);
    
    // add these buttons to a group
    // for the "select" and "unselect" functionality
    vector<UIButton*>* mainButtonGroup = new vector<UIButton*>();
    mainButtonGroup->push_back(telepresenceModeButton);
    mainButtonGroup->push_back(wavyModeButton);
    mainButtonGroup->push_back(threeDModeButton);
    mainButtonGroup->push_back(mathModeButton);
    uiHandler->addButtonGroup(mainButtonGroup);
    
    UITriggers::buttonTrigger(telepresenceModeButton);
}

//--------------------------------------------------------------

void GUI::setupLeftSidebar(UIHandler *uiHandler, const int leftSidebarWidth) {
    setupInfoBar(uiHandler, leftSidebarWidth);
    setupTeleBar(uiHandler, leftSidebarWidth);
    setupThreeDBar(uiHandler, leftSidebarWidth);
}

//--------------------------------------------------------------

void GUI::setupInfoBar(UIHandler *uiHandler, const int leftSidebarWidth) {
    // left sidebar stuff:
    // info screen
    // inFORM logo
    
    ofImage* inFORMLogoImg = new ofImage();
    inFORMLogoImg->loadImage("inform-logo.png");
    
    const int informLogoWidth = inFORMLogoImg->getWidth();
    const int informLogoHeight = inFORMLogoImg->getHeight();
    const int informLogoX = leftSidebarWidth/2 - informLogoWidth/2;
    const int informLogoY = informLogoX;
    
    UIImage *inFORMLogo = new UIImage(inFORMLogoImg, informLogoX, informLogoY);
    
    
    // creator text
    const int creatorTextWidth = 270;
    const int creatorTextX = leftSidebarWidth/2 - creatorTextWidth/2;
    const int creatorTextY = informLogoY + informLogoHeight + 25;
    const int creatorTextSize = 9;
    const string creatorTextStr = "Daniel Leithinger, Sean Follmer, Alex Olwal, Akimitsu Hogge, Hiroshi Ishii";
    
    UIText* creatorText = new UIText(creatorTextStr, "creators", creatorTextSize, creatorTextX, creatorTextY);
    creatorText->setAlignment("left");
    creatorText->setWidth(creatorTextWidth);
    creatorText->setColor(ofColor(175,175,175));
    
    // inFORM description
    const int descTextWidth = 300;
    const int descTextX = leftSidebarWidth/2 - descTextWidth/2;
    const int descTextY = creatorTextY + 100;
    const int descTextSize = 10;
    const string descTextStr = "The inFORM is a Dynamic Shape Display that can render 3D content physically, so users can interact with digital information in a tangible way while also interacting with the physical world around it.";
    
    UIText* inFORMText = new UIText(descTextStr, "inFORM description", descTextSize, descTextX, descTextY);
    inFORMText->setAlignment("left");
    inFORMText->setWidth(descTextWidth);
    inFORMText->setColor(ofColor(45,45,45));
    
    // divider
    ofImage* dividerImg = new ofImage();
    dividerImg->loadImage("divider.png");
    
    const int dividerWidth = dividerImg->getWidth();
    const int dividerHeight = dividerImg->getHeight();
    const int dividerX = leftSidebarWidth/2 - dividerWidth/2;
    const int dividerY = descTextY + 210;
    
    UIImage* divider = new UIImage("divider.png", dividerX, dividerY);
    
    // tmg logo
    ofImage* tmgLogImg = new ofImage();
    tmgLogImg->loadImage("tmg-logo.png");
    
    const int tmgLogoWidth = tmgLogImg->getWidth();
    const int tmgLogoHeight = tmgLogImg->getHeight();
    const int tmgLogoX = leftSidebarWidth/2 - tmgLogoWidth/2;
    const int tmgLogoY = dividerY + 50 + dividerHeight;
    
    UIImage *tmgLogo = new UIImage(tmgLogImg, tmgLogoX, tmgLogoY);
    
    // tmg description
    const int tmgTextWidth = 300;
    const int tmgTextX = leftSidebarWidth/2 - descTextWidth/2;
    const int tmgTextY = tmgLogoY + tmgLogoHeight + 50;
    const int tmgTextSize = 10;
    const string tmgTextStr = "The Tangible Media Group, led by Professor Hiroshi Ishii, seeks to seamlessly couple the dual world of bits and atoms by giving dynamic physical form to digital information and computation.";
    UIText* tmgText = new UIText(tmgTextStr, "TMG description", tmgTextSize, tmgTextX, tmgTextY);
    tmgText->setAlignment("left");
    tmgText->setWidth(tmgTextWidth);
    tmgText->setColor(ofColor(45,45,45));
    
    uiHandler->addImage(inFORMLogo);
    uiHandler->addText(creatorText);
    uiHandler->addText(inFORMText);
    uiHandler->addImage(divider);
    uiHandler->addImage(tmgLogo);
    uiHandler->addText(tmgText);
    
    
    vector<UIElement*> infoScreenGroup = vector<UIElement*>();
    infoScreenGroup.push_back(inFORMLogo);
    infoScreenGroup.push_back(creatorText);
    infoScreenGroup.push_back(inFORMText);
    infoScreenGroup.push_back(divider);
    infoScreenGroup.push_back(tmgLogo);
    infoScreenGroup.push_back(tmgText);
    uiHandler->addUIGroup(infoScreenGroup, "info");
}

//--------------------------------------------------------------

void GUI::setupTeleBar(UIHandler *uiHandler, const int leftSidebarWidth) {
    // telepresence stuff
    ofImage* teleBatImage = new ofImage();
    teleBatImage->loadImage("Teleoperation/assets/bat.png");
    
    const int teleBatWidth = teleBatImage->getWidth();
    const int teleBatX = leftSidebarWidth/2 - teleBatWidth/2;
    const int teleBatY = 50;
    
    UIImage *teleBat = new UIImage(teleBatImage, teleBatX, teleBatY);
    
    
    const int telePushY = teleBatY + 175;
    ofImage* telePushImage = new ofImage();
    telePushImage->loadImage("Teleoperation/assets/push.png");
    
    teleBat->setCaption("Bat the ball left and right", 10, 300);
    
    UIImage *telePush = new UIImage(telePushImage, teleBatX, telePushY);
    
    telePush->setCaption("Cup your hand and push", 10, 300);
    
    ofImage* teleScoopImage = new ofImage();
    teleScoopImage->loadImage("Teleoperation/assets/scoop.png");
    const int teleScoopY = teleBatY + 2*175;
    UIImage *teleScoop = new UIImage(teleScoopImage, teleBatX, teleScoopY);
    
    teleScoop->setCaption("Start low and scoop it", 10, 300);
    
    uiHandler->addImage(teleBat);
    uiHandler->addImage(telePush);
    uiHandler->addImage(teleScoop);
    
    vector<UIElement*> telepresenceGroup = vector<UIElement*>();
    telepresenceGroup.push_back(teleBat);
    telepresenceGroup.push_back(telePush);
    telepresenceGroup.push_back(teleScoop);
    uiHandler->addUIGroup(telepresenceGroup, "telepresence");
}

//--------------------------------------------------------------

void GUI::setupThreeDBar(UIHandler* uiHandler, const int leftSidebarWidth)
{
    //BUTTONS
    // UIButton name = UIButton("name", x,y, w,h)
    const int modelSelectButtonWidth  = 142;
    const int modelSelectButtonHeight = 70;
    
    const int modelSelectButtonX = 20;
    const int modelSelectButtonY = 300;
    const int spacer = 200;
    
    UIButton* modelSelectButtonLeft
    = new UIButton("modelSelectLeft",
                   modelSelectButtonX, modelSelectButtonY,
                   modelSelectButtonWidth,modelSelectButtonHeight);
    modelSelectButtonLeft->setImageIdle("3D Models/assets/larrow.png");
    modelSelectButtonLeft->setImageActive("3D Models/assets/larrow.png");
    modelSelectButtonLeft->setImageSelected("3D Models/assets/larrow.png");
    
    UIButton* modelSelectButtonRight
    = new UIButton("modelSelectRight",
                   modelSelectButtonX + spacer, modelSelectButtonY,
                   modelSelectButtonWidth,modelSelectButtonHeight);
    modelSelectButtonRight->setImageIdle("3D Models/assets/rarrow.png");
    modelSelectButtonRight->setImageActive("3D Models/assets/rarrow.png");
    modelSelectButtonRight->setImageSelected("3D Models/assets/rarrow.png");
    
    
    /*
    ofImage* modelSelectButtonLeftImg = new ofImage();
    modelSelectButtonLeftImg->loadImage("3D Models/assets/larrow.png");
    
    ofImage* modelSelectButtonRightImg = new ofImage();
    modelSelectButtonRightImg->loadImage("3D Models/assets/rarrow.png");
    
    const int selectButtonWidth = modelSelectButtonLeftImg->getWidth();
    const int selectButtonHeight = modelSelectButtonLeftImg->getHeight();
    const int selectButtonX = leftSidebarWidth/2 - selectButtonWidth/2;
    const int selectButtonY = selectButtonX;
    
    UIImage *modelSelect = new UIImage(modelSelectButtonLeftImg, informLogoX, informLogoY)
    */
    
    // SLIDERS
    const int sliderWidth = 300;
    const int sliderX = MARGIN_X/2 - sliderWidth/2;
    const int sliderY = 1080 * 2/4;
    
    // name, bool horizontal, int trackX, int trackY, int trackLength, int handleWidth, int handleHeight
    //3D SLIDER
    UISlider *sliderScale = new UISlider("sliderScale", true, sliderX, sliderY, sliderWidth, 46, 50);
    sliderScale->setImageHandleActive("3D Models/assets/knob.png");
    sliderScale->setImageHandleIdle("3D Models/assets/knob.png");
    sliderScale->setImageTrack("3D Models/assets/slider.png");
    sliderScale->setInitialPos(0.5);
    
    
    uiHandler->addButton(modelSelectButtonLeft);
    uiHandler->addButton(modelSelectButtonRight);
    uiHandler->addSlider(sliderScale);
    
    vector<UIElement*> threeDGroup = vector<UIElement*>();
    threeDGroup.push_back(modelSelectButtonLeft);
    threeDGroup.push_back(modelSelectButtonRight);
    threeDGroup.push_back(sliderScale);
    
    uiHandler->addUIGroup(threeDGroup, "3D");
}

//--------------------------------------------------------------

void GUI::setupUI(UIHandler* uiHandler) {
    const int leftSidebarWidth = 420;
    const int rightSidebarWidth = 420;
    
    setupRightSidebar(uiHandler, rightSidebarWidth);
    
    setupLeftSidebar(uiHandler, leftSidebarWidth);
    
    // drop shadow for middle region
    ofImage* centerDropShadowImg = new ofImage();
    centerDropShadowImg->loadImage("pin-shadow.png");
    
    const int dropShadowX = leftSidebarWidth + 1;
    const int dropShadowY = 1;
    
    UIImage *centerDropShadow = new UIImage(centerDropShadowImg, dropShadowX, dropShadowY);
    
    uiHandler->addImage(centerDropShadow);
}

//--------------------------------------------------------------

