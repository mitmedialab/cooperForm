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
    setupWaveBar(uiHandler, leftSidebarWidth);
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
    inFORMText->setColor(ofColor(35,35,35));
    
    // divider
    ofImage* dividerImg = new ofImage();
    dividerImg->loadImage("divider.png");
    
    const int dividerWidth = dividerImg->getWidth();
    const int dividerHeight = dividerImg->getHeight();
    const int dividerX = leftSidebarWidth/2 - dividerWidth/2;
    const int dividerY = descTextY + 210;
    
    UIImage* divider = new UIImage(dividerImg, dividerX, dividerY);
    
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
    tmgText->setColor(ofColor(35,35,35));
    
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
    
    // divider
    ofImage* dividerImg = new ofImage();
    dividerImg->loadImage("divider.png");
    
    const int dividerWidth = dividerImg->getWidth();
    const int dividerHeight = dividerImg->getHeight();
    const int dividerX = leftSidebarWidth/2 - dividerWidth/2;
    
    // headline text
    const int textWidth = 300;
    const int textX = leftSidebarWidth/2 - textWidth/2;
    const int headlineY = 60;
    const int descY = 105;
    
    const int headlineSize = 16;
    const int textSize = 10;
    
    const string headlineStr = "Teleoperation Tips";
    const string teleDescStr = "inFORM virtually mirrors your hands to directly control and play with the red ball. Here are some fun things to try out:";
    
    UIText* headline = new UIText(headlineStr, "tele headline", headlineSize, textX, headlineY);
    headline->setWidth(textWidth);
    headline->setColor(ofColor(35,35,35));
    UIText* teleDesc =new UIText(teleDescStr, "tele description", textSize, textX, descY);
    teleDesc->setWidth(textWidth);
    teleDesc->setColor(ofColor(35,35,35));
    
    const int divider1Y = descY + 125;
    UIImage* divider1 = new UIImage(dividerImg,
                                    dividerX, divider1Y);
    
    // telepresence stuff
    ofImage* teleBatImage = new ofImage();
    teleBatImage->loadImage("Teleoperation/assets/bat.png");
    
    const int teleBatHeight = teleBatImage->getHeight();
    const int teleBatWidth = teleBatImage->getWidth();
    const int teleBatX = leftSidebarWidth/2 - teleBatWidth/2;
    const int teleBatY = divider1Y + 25;
    const int teleSpacing = 60;
    const int captionTextHeight = 30;
    
    UIImage *teleBat = new UIImage(teleBatImage, teleBatX, teleBatY);
    teleBat->setCaption("Bat the ball left and right", 10, 300);
    teleBat->getCaption()->setColor(ofColor(35,35,35));
    
    const int divider2Y = teleBatY + teleBatHeight + captionTextHeight + teleSpacing/2;
    UIImage* divider2 = new UIImage(dividerImg,
                                    dividerX, divider2Y);
    
    const int telePushY = teleBatY + teleBatHeight + captionTextHeight + teleSpacing;
    ofImage* telePushImage = new ofImage();
    telePushImage->loadImage("Teleoperation/assets/push.png");
    
    UIImage *telePush = new UIImage(telePushImage, teleBatX, telePushY);
    telePush->setCaption("Cup your hand and push", 10, 300);
    telePush->getCaption()->setColor(ofColor(35,35,35));
    
    const int divider3Y = telePushY + teleBatHeight + captionTextHeight + teleSpacing/2;
    UIImage* divider3 = new UIImage(dividerImg,
                                    dividerX, divider3Y);
    
    ofImage* teleScoopImage = new ofImage();
    teleScoopImage->loadImage("Teleoperation/assets/scoop.png");
    
    const int teleScoopY = telePushY + teleBatHeight + captionTextHeight + teleSpacing;
    UIImage *teleScoop = new UIImage(teleScoopImage, teleBatX, teleScoopY);
    teleScoop->setCaption("Start low and scoop it", 10, 300);
    teleScoop->getCaption()->setColor(ofColor(35,35,35));
    
    uiHandler->addText(headline);
    uiHandler->addText(teleDesc);
    uiHandler->addImage(divider1);
    uiHandler->addImage(teleBat);
    uiHandler->addImage(divider2);
    uiHandler->addImage(telePush);
    uiHandler->addImage(divider3);
    uiHandler->addImage(teleScoop);
    
    
    vector<UIElement*> telepresenceGroup = vector<UIElement*>();
    telepresenceGroup.push_back(headline);
    telepresenceGroup.push_back(teleDesc);
    telepresenceGroup.push_back(divider1);
    telepresenceGroup.push_back(teleBat);
    telepresenceGroup.push_back(divider2);
    telepresenceGroup.push_back(telePush);
    telepresenceGroup.push_back(divider3);
    telepresenceGroup.push_back(teleScoop);
    uiHandler->addUIGroup(telepresenceGroup, "telepresence");
}

//--------------------------------------------------------------

void GUI::setupWaveBar(UIHandler *uiHandler, const int leftSidebarWidth) {
    
    // divider
    ofImage* dividerImg = new ofImage();
    dividerImg->loadImage("divider.png");
    
    const int dividerWidth = dividerImg->getWidth();
    const int dividerHeight = dividerImg->getHeight();
    const int dividerX = leftSidebarWidth/2 - dividerWidth/2;
    
    // headline text
    const int textWidth = 300;
    const int textX = leftSidebarWidth/2 - textWidth/2;
    const int headlineY = 60;
    const int descY = 105;
    
    const int headlineSize = 16;
    const int textSize = 10;
    
    const string headlineStr = "Wave Tips";
    const string waveDescStr = "Wave your hands over the table on the right to create various water ripples across the remote tabletop.";
    
    UIText* headline = new UIText(headlineStr, "wave headline", headlineSize, textX, headlineY);
    headline->setWidth(textWidth);
    headline->setColor(ofColor(35,35,35));
    UIText* waveDesc =new UIText(waveDescStr, "wave description", textSize, textX, descY);
    waveDesc->setWidth(textWidth);
    waveDesc->setColor(ofColor(35,35,35));
    
    const int divider1Y = descY + 125;
    UIImage* divider1 = new UIImage(dividerImg,
                                    dividerX, divider1Y);
    
    
    
    // wavepresence stuff
    ofImage* waveSideImage = new ofImage();
    waveSideImage->loadImage("Wave/assets/side.png");
    
    const int waveSideHeight = waveSideImage->getHeight();
    const int waveSideWidth = waveSideImage->getWidth();
    const int waveSideX = leftSidebarWidth/2 - waveSideWidth/2;
    const int waveSideY = divider1Y + 25;
    const int waveSpacing = 60;
    const int captionTextHeight = 30;
    
    UIImage *waveSide = new UIImage(waveSideImage, waveSideX, waveSideY);
    waveSide->setCaption("Move your hand side to side", 10, 300);
    waveSide->getCaption()->setColor(ofColor(35,35,35));
    
    const int divider2Y = waveSideY + waveSideHeight + captionTextHeight + waveSpacing/2;
    UIImage* divider2 = new UIImage(dividerImg,
                                    dividerX, divider2Y);
    
    const int waveVerticalY = waveSideY + waveSideHeight + captionTextHeight + waveSpacing;
    ofImage* waveVerticalImage = new ofImage();
    waveVerticalImage->loadImage("Wave/assets/height.png");
    
    UIImage *waveVertical = new UIImage(waveVerticalImage, waveSideX, waveVerticalY);
    waveVertical->setCaption("Up & down for stronger waves", 10, 300);
    waveVertical->getCaption()->setColor(ofColor(35,35,35));
    
    const int divider3Y = waveVerticalY + waveSideHeight + captionTextHeight + waveSpacing/2;
    UIImage* divider3 = new UIImage(dividerImg,
                                    dividerX, divider3Y);
    
    ofImage* waveBothImage = new ofImage();
    waveBothImage->loadImage("Wave/assets/both.png");
    
    const int waveBothY = waveVerticalY + waveSideHeight + captionTextHeight + waveSpacing;
    UIImage *waveBoth = new UIImage(waveBothImage, waveSideX, waveBothY);
    waveBoth->setCaption("Try both hands at once", 10, 300);
    waveBoth->getCaption()->setColor(ofColor(35,35,35));
    
    uiHandler->addText(headline);
    uiHandler->addText(waveDesc);
    uiHandler->addImage(divider1);
    uiHandler->addImage(waveSide);
    uiHandler->addImage(divider2);
    uiHandler->addImage(waveVertical);
    uiHandler->addImage(divider3);
    uiHandler->addImage(waveBoth);
    
    
    vector<UIElement*> wavepresenceGroup = vector<UIElement*>();
    wavepresenceGroup.push_back(headline);
    wavepresenceGroup.push_back(waveDesc);
    wavepresenceGroup.push_back(divider1);
    wavepresenceGroup.push_back(waveSide);
    wavepresenceGroup.push_back(divider2);
    wavepresenceGroup.push_back(waveVertical);
    wavepresenceGroup.push_back(divider3);
    wavepresenceGroup.push_back(waveBoth);
    uiHandler->addUIGroup(wavepresenceGroup, "wavy");
}

//--------------------------------------------------------------

void GUI::setupThreeDBar(UIHandler* uiHandler, const int leftSidebarWidth)
{
    //TEXT
    const int textWidth = 300;
    const int textX = 60;
    const int textY = 105;
    const int headlineY = 60;
    const int captionX = 88;
    const int caption1Y = 300;
    const int caption2Y = 570;
    const int caption3Y = 840;
    const int modelNameY = 380;
    
    const int headlineSize = 16;
    const int textSize = 10;
    const int modelNameSize = 20;
    
    const string headlineStr = "3d Model Controls";
    const string textStr = "Use the touch screen controls located below to load a new 3d model, rotate it around, and/or scale it to different sizes.";
    const string caption1Str = "Select between 3d models";
    const string caption2Str = "Slide Finger below to rotate";
    const string caption3Str = "Drag slider to resize model";
    //string modelNameStr = ((ReliefApplication*)ofGetAppPtr())->threeDShapeObject->getCurrentModelName(); // could be nicer
    const string modelNameStr = " "; //will be changed dynamically
    
    UIText* text = new UIText(textStr, "3D description", textSize, textX, textY);
    UIText* headline = new UIText(headlineStr, "3d headline", headlineSize, textX, headlineY);
    UIText* caption1 = new UIText(caption1Str, "caption1", textSize, captionX, caption1Y);
    UIText* caption2 = new UIText(caption2Str, "caption2", textSize, captionX, caption2Y);
    UIText* caption3 = new UIText(caption3Str, "caption3", textSize, captionX, caption3Y);
    UIText* modelName = new UIText(modelNameStr, "modelName", modelNameSize, textX, modelNameY);
    
    headline->setAlignment("left");
    headline->setWidth(textWidth);
    headline->setColor(ofColor(35,35,35));
    text->setAlignment("left");
    text->setWidth(textWidth);
    text->setColor(ofColor(35,35,35));
    caption1->setAlignment("left");
    caption1->setWidth(textWidth);
    caption1->setColor(ofColor(35,35,35));
    caption2->setAlignment("left");
    caption2->setWidth(textWidth);
    caption2->setColor(ofColor(35,35,35));
    caption3->setAlignment("left");
    caption3->setWidth(textWidth);
    caption3->setColor(ofColor(35,35,35));
    modelName->setAlignment("center");
    modelName->setWidth(textWidth);
    modelName->setColor(ofColor(35,35,35));

    
    // DIVIDERS
    ofImage* dividerImg = new ofImage();
    dividerImg->loadImage("divider.png");
    
    const int dividerWidth = dividerImg->getWidth();
    const int dividerHeight = dividerImg->getHeight();
    const int dividerX = leftSidebarWidth/2 - dividerWidth/2;
    const int dividerY = 270;
    const int dividerSpacerY = 270;
    
    UIImage* divider1 = new UIImage(dividerImg, dividerX, dividerY);
    UIImage* divider2 = new UIImage(dividerImg, dividerX, dividerY + 1 * dividerSpacerY);
    UIImage* divider3 = new UIImage(dividerImg, dividerX, dividerY + 2 * dividerSpacerY);
    
    
    //BUTTONS
    // UIButton name = UIButton("name", x,y, w,h)
    const int modelSelectButtonWidth  = 142;
    const int modelSelectButtonHeight = 70;
    
    const int modelSelectButtonX = 60;
    const int modelSelectButtonY = 440;
    const int spacer = 160;
    
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
    
    
    // SLIDERS
    const int sliderWidth = 280;
    const int sliderX = MARGIN_X/2 - sliderWidth/2;
    const int sliderY = 940;
    
    // name, bool horizontal, int trackX, int trackY, int trackLength, int handleWidth, int handleHeight
    //3D SLIDER
    UISlider *sliderScale = new UISlider("sliderScale", true, sliderX, sliderY, sliderWidth, 46, 50);
    sliderScale->setImageHandleActive("3D Models/assets/knob.png");
    sliderScale->setImageHandleIdle("3D Models/assets/knob.png");
    sliderScale->setImageTrack("3D Models/assets/slider.png");
    sliderScale->setImageTrackOffset(-10);
    sliderScale->setHandlePos(0.5);
    
    sliderScale->setLockToPosThresh(0.05);
    sliderScale->addLockToPos(0.5);
    sliderScale->addLockToPos(0);
    sliderScale->addLockToPos(1);
    
    //SETUP
    uiHandler->addButton(modelSelectButtonLeft);
    uiHandler->addButton(modelSelectButtonRight);
    uiHandler->addSlider(sliderScale);
    uiHandler->addImage(divider1);
    uiHandler->addImage(divider2);
    uiHandler->addImage(divider3);
    uiHandler->addText(headline);
    uiHandler->addText(text);
    uiHandler->addText(caption1);
    uiHandler->addText(caption2);
    uiHandler->addText(caption3);
    uiHandler->addText(modelName);

    vector<UIElement*> threeDGroup = vector<UIElement*>();
    threeDGroup.push_back(modelSelectButtonLeft);
    threeDGroup.push_back(modelSelectButtonRight);
    threeDGroup.push_back(sliderScale);
    threeDGroup.push_back(divider1);
    threeDGroup.push_back(divider2);
    threeDGroup.push_back(divider3);
    threeDGroup.push_back(headline);
    threeDGroup.push_back(text);
    threeDGroup.push_back(caption1);
    threeDGroup.push_back(caption2);
    threeDGroup.push_back(caption3);
    threeDGroup.push_back(modelName);
    
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

