//
//  KinectShapeObject.h
//  basicExample
//
//  Created by Daniel Leithinger on 3/20/14.
//
//

#ifndef __basicExample__KinectShapeObject__
#define __basicExample__KinectShapeObject__

#include "ShapeObject.h"
#include "KinectTracker.h"
#include "ofxOpenCv.h"
#include <iostream>

class KinectShapeObject : public ShapeObject {
public:
    KinectShapeObject();
    
    
    // Update the current shape object
    void update();
    
    // draw the depth map for given shape
    // region will be from (0,0) to (w,h)
    void renderTangibleShape(int w, int h);
    
    // draw the graphics for the UI
    void renderTouchscreenGraphics(int w, int h);
    
    // draw the graphics for the vertical back screen
    void renderVerticalScreenGraphics(int w, int h);
    
    
    // mouse events from the touch screen
    virtual void mouseMoved(int x, int y);
    virtual void mousePressed(int x, int y, int button);
    
    void setKinectTracker(KinectTracker * pKinectTracker){mKinectTracker = pKinectTracker;};
    void setTableValuesForShape(ShapeIOManager *pIOManager);

    
private:
    KinectTracker * mKinectTracker;
    
    ofFbo mKinectHeightImage, mOutputShapeImage;
    
    ofImage * kinectHeightImage;
    ofImage * outputShapeImage;
};

#endif /* defined(__basicExample__KinectShapeObject__) */
