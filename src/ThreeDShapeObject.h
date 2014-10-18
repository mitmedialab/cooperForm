//
//  ThreeDShapeObject.h
//  cooperFORM
//
//  Created by Philipp Schoessler on 10/17/14.
//
//

#ifndef __cooperFORM__ThreeDShapeObject__
#define __cooperFORM__ThreeDShapeObject__

#include "ofxAssimpModelLoader.h"
#include "ofx3DModelLoader.h"
#include "ShapeObject.h"
#include <iostream>

class ThreeDShapeObject : public ShapeObject {
    
public:
    ThreeDShapeObject();
    void update();
    void renderProjectorOverlay(int w, int h);
    void renderTangibleShape(int w, int h);
    void renderTouchScreenGraphics(int w, int h);
    void renderMarginGraphics(int x, int y);
    
    void setMouseDragInfo(int x, int y, int button);
    void setMousePressedInfo(int x, int y);
    void reset();
    
    ofxAssimpModelLoader model;
    ofCamera cam;
    ofLight	light;
    ofVboMesh vbo;
    ofFbo fbo;
    
    string get_shape_name() {return shape_name; };
    void setTableValuesForShape(ShapeIOManager *pIOManager) {};
    
    
    ofImage bgImg;
    
    ofShader shader;
    
    //arcball
    ofQuaternion    quat;
    ofVec2f         oldMouse;
    float           dampen;
    ofVec3f axis;
    float angle;
    
    int marginX;
    int marginY;
    
private:
    string shape_name = "3D";

};

#endif /* defined(__cooperFORM__ThreeDShapeObject__) */
