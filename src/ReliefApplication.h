#pragma once

#include "ofMain.h"
#include "UIHandler.h"
#include "UIButton.h"
#include "ShapeIOManager.h"
#include "ShapeObject.h"
#include "CameraTracker.h"
#include "KinectShapeObject.h"
#include "WavyShapeObject.h"
#include "KinectTracker.h"

class ReliefApplication : public ofBaseApp {
public:
    UIHandler       uiHandler;
    UIButton        * telepresenceModeButton;
    UIButton        * wavyModeButton;
    UIButton        * threeDModeButton;
    UIButton        * mathModeButton;

    // the table mode we're on
    // "telepresence", "wavy", "3D", and "math"
    string          currentMode;
    
    // manages communication with the pin display
    ShapeIOManager   * mIOManager;

    // table config variables
    float            gain_P;
    float            gain_I;
    int              max_I;
    int              deadZone;
    int              maxSpeed;

    // heightmap data for the pins 
    ofFbo            pinHeightMapImage;
    // the heightmap data for the pins
    ofFbo            pinHeightMapImageSmall;
    // image to be projected on top of the pins
    ofFbo            projectorOverlayImage;
    // image to be displayed on the touch screen
    ofFbo            touchScreenDisplayImage;
    // image to be displayed on the vertical display
    ofFbo            verticalDisplayImage;

    ShapeObject      * currentShape;

    // our interface for the cameras
    CameraTracker    cameraTracker;

    // Shape objects
    KinectShapeObject  * kinectShapeObject;
    WavyShapeObject    * wavyShapeObject;

    // our interface for the Kinect
    KinectTracker    kinectTracker;
    
    void setup();
    void update();
    void draw();
    void exit();
    
    // initialize all the UI elements
    void setupUI();
    
    void setMode(string newMode);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
};
