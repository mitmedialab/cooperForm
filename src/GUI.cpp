//
//  GUI.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 10/10/14.
//
//

// Constructs the GUI

#include "GUI.h"
void GUI::setupUI(UIHandler* uiHandler) {
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
    
    
    
    // left sidebar stuff:
    // inFORM logo
    const int sideBarLeftSize = 420;
    
    ofImage* inFORMLogoImg = new ofImage();
    inFORMLogoImg->loadImage("inform-logo.png");
    
    const int informLogoWidth = inFORMLogoImg->getWidth();
    const int informLogoHeight = inFORMLogoImg->getHeight();
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
    const int sliderY = 1080 * 2/4;
    // name, bool horizontal, int trackX, int trackY, int trackLength, int handleWidth, int handleHeight
    UISlider *slider = new UISlider("slider", true, sliderX, sliderY, sliderWidth, 46, 50);
    slider->setImageHandleActive("3D Models/assets/knob.png");
    slider->setImageHandleIdle("3D Models/assets/knob.png");
    slider->setImageTrack("3D Models/assets/slider.png");
    
    // telepresence stuff
    ofImage* teleBatImage = new ofImage();
    teleBatImage->loadImage("Teleoperation/assets/bat.png");
    
    const int teleBatWidth = teleBatImage->getWidth();
    const int teleBatX = sideBarLeftSize/2 - teleBatWidth/2;
    const int teleBatY = informLogoY + informLogoHeight + 50;
    
    UIImage *teleBat = new UIImage(teleBatImage, teleBatX, teleBatY);
    
    ofImage* telePushImage = new ofImage();
    telePushImage->loadImage("Teleoperation/assets/push.png");
    
    const int telePushY = teleBatY + 175;
    
    UIImage *telePush = new UIImage(telePushImage, teleBatX, telePushY);
    
    ofImage* teleScoopImage = new ofImage();
    teleScoopImage->loadImage("Teleoperation/assets/scoop.png");
    const int teleScoopY = teleBatY + 2*175;
    UIImage *teleScoop = new UIImage(teleScoopImage, teleBatX, teleScoopY);
    
    
    const int textWidth = 300;
    const int textX = sideBarLeftSize/2 - textWidth/2;
    const int textY = informLogoY + informLogoHeight + 50;
    const int textSize = 10;
    string text = "The inFORM is a Dynamic Shape Display that can render 3D content physically, so users can interact with digital information in a tangible way while also interacting with the physical world around it.";
    UIText* inFORMText = new UIText(text, "inFORM description", textSize, textX, textY);
    inFORMText->setAlignment("left");
    inFORMText->setWidth(textWidth);
    inFORMText->setColor(ofColor(0,0,0));
    
    //uiHandler.addImage(teleBat);
    //uiHandler.addImage(telePush);
    //uiHandler.addImage(teleScoop);
    uiHandler->addImage(inFORMLogo);
    uiHandler->addImage(tmgLogo);
    uiHandler->addText(inFORMText);
    uiHandler->addSlider(slider);
    
    vector<UIElement*> telepresenceGroup = vector<UIElement*>();
    telepresenceGroup.push_back(teleBat);
    telepresenceGroup.push_back(telePush);
    telepresenceGroup.push_back(teleScoop);
    uiHandler->addUIGroup(telepresenceGroup, "telepresence");
    
    
    // drop shadow for middle region
    ofImage* centerDropShadowImg = new ofImage();
    centerDropShadowImg->loadImage("pin-shadow.png");
    
    const int dropShadowX = sideBarLeftSize + 1;
    const int dropShadowY = 1;
    
    UIImage *centerDropShadow = new UIImage(centerDropShadowImg, dropShadowX, dropShadowY);
    
    uiHandler->addImage(centerDropShadow);
    
    
    UITriggers::buttonTrigger(telepresenceModeButton);
}