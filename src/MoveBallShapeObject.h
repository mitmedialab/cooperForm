//
//  MoveBallShapeObject.h
//  cooperFORM
//
//  Created by Tangible Media Group on 10/13/14.
//
//

#ifndef __cooperFORM__MoveBallShapeObject__
#define __cooperFORM__MoveBallShapeObject__

/*
 Will move the ball to the corner or center.
 */
#include "ShapeObject.h"
#include <iostream>

class MoveBallShapeObject : public ShapeObject {
public:
    MoveBallShapeObject();
    
    void setTableValuesForShape(ShapeIOManager *pIOManager){};
    
    void update();
    
    void renderTangibleShape(int w, int h);
    
    
    void renderTouchscreenGraphics(int w, int h);
    
    void renderProjectorOverlay(int w, int h){};
    
    void mouseMoved(int x, int y){};
    void mousePressed(int x, int y, int button){};
    
    void moveBallToCenter();
    void moveBallToCorner();
    
    bool isBallInCorner();
private:
    // buffer to modify
    bool toCenter = true;
    ofFbo depthBuffer;
    long startAnimationTime;
    long toCornerAnimationTime = 3000;
    long toCenterAnimationTime = 1000;
    
    int cornerX = 20;
    int cornerY = 100-20;
    
    bool ballInCorner = false;
};
#endif /* defined(__cooperFORM__MoveBallShapeObject__) */
