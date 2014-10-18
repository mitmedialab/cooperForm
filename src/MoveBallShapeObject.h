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
    
    
    void renderTouchScreenGraphics(int w, int h);
    
    void renderProjectorOverlay(int w, int h){};
    
    void mouseMoved(int x, int y){};
    void mousePressed(int x, int y, int button){};
    
    void moveBallToCenter();
    void moveBallToCorner();
    
private:
    // buffer to modify
    ofFbo depthBuffer;
    long startAnimationTime;
    long animationTime = 4500;
    
    // true if moving ball to center
    // false if moving ball to corner
    bool toCorner;
};
#endif /* defined(__cooperFORM__MoveBallShapeObject__) */
