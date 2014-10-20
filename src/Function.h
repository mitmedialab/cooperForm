//
//  Function.h
//  cooperFORM
//
//  Created by Philipp Schoessler on 10/19/14.
//
//

#ifndef __cooperFORM__Function__
#define __cooperFORM__Function__

#include "RenderObject.h"

class Function : public RenderObject {
public:
    Function();
    
    virtual void drawHeightMap(int w, int h);
    virtual void drawGraphics(int w, int h);
    virtual void drawEquation();
    virtual void adjustVar(int num, int change);
    virtual void update(float dt);
    
    ofVboMesh mainMesh;
    int extrusionAmount;
    //virtual void drawText();
    
    void nextFunction(bool t);
    
    float eq_val1;
    float eq_val2;
    
    int ratio;
    
    ofTrueTypeFont text;
    ofCamera cam;
    ofEasyCam easycam;

private:
    float prev[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    float curr[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    float next[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    unsigned short currFunc;
    // vector<ofImage> images;
    ofxAnimatableFloat anim;
    
    bool t;
};

#endif /* defined(__cooperFORM__Function__) */
