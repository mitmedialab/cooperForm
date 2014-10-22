//
//  MathShapeObject.h
//  cooperFORM
//
//  Created by Philipp Schoessler on 10/19/14.
//
//

#ifndef __cooperFORM__MathShapeObject__
#define __cooperFORM__MathShapeObject__

#include "ShapeObject.h"
#include "Function.h"

class MathShapeObject : public ShapeObject {
    
 //   struct mathEquation
  //  {
   //     ofxAssimpModelLoader model;
    //    string name;
   // };
    
public:
    MathShapeObject();
    void update();
    void renderProjectorOverlay(int w, int h);
    void renderTangibleShape(int w, int h);
    void renderTouchscreenGraphics(int w, int h);
    void keyReleased(int key);
    
    void nextFunction();
    void chooseFunction(int func);
    
    
    void modifyVal1Up();
    void modifyVal1Down();
    void modifyVal2Up();
    void modifyVal2Down();
    
    Function function;
    
    string getEqVal1();
    string getEqVal2();
    
    string get_shape_name() {return shape_name; };
    //void setTableValuesForShape(ShapeIOManager *pIOManager);
    
private:
    string shape_name = "math";
    
    ofFbo touchscreenImage; //FBO where we render graphics for pins
    ofFbo pinHeightMapImage; //FBO where we render height map
    ofFbo projectorImage; //FBO that will be displayed from the projector
};


#endif /* defined(__cooperFORM__MathShapeObject__) */
