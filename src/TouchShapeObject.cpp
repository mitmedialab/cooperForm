//
//  TouchShapeObject.cpp
//  basicExample
//
//  Created by Ken Nakagaki on 6/16/15.
//
//

#include "TouchShapeObject.h"


TouchShapeObject::TouchShapeObject()
{
    //mKinectHeightImage.allocate(KINECT_X, KINECT_Y);
    mOutputShapeImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    allPixels = new unsigned char[RELIEF_SIZE];
    
    
    for(int i = 0; i< RELIEF_SIZE; i++){
        for(int j = 0; j< filterFrame; j++){
            allPixels_store[i][j] = 210;
        }
    }
    
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            differenceHeight[i][j] = 0;
        }
    }
    
}

void TouchShapeObject::setup()
{
    
}

//----------------------------------------------------

void TouchShapeObject::update() {
    update(0.1);
}

void TouchShapeObject::update(float dt)
{
    unsigned char * pixels;
    int lineSize = sizeof(char) * RELIEF_SIZE_Y;
    
    
    //store data
    for(int i = 0; i < RELIEF_SIZE; i++){
        for(int j = filterFrame-1; j > 0; j--){
            allPixels_store[i][j] = allPixels_store[i][j-1];
        }
        allPixels_store[i][0] = allPixels[i];
    }
    
    //check each pin's difference over time - if it's flat or not
    Boolean flat[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    for(int i = 0; i< RELIEF_SIZE_X; i++){

        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            
            int maxVal=0, minVal=255;
            for(int k =0; k < filterFrame; k++){
                maxVal = MAX(maxVal,(int)allPixels_store[RELIEF_PHYSICAL_SIZE_X*j+i][k]);
                minVal = MIN(minVal,(int)allPixels_store[RELIEF_PHYSICAL_SIZE_X*j+i][k]);
            }
            if (abs(maxVal - minVal) > 2) {
                flat[i][j] = false;
            } else {
                flat[i][j] = true;
            }
            
        }
    }

    
    
    // caliculate difference between send and receive value
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){

            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+i]);
            int input = int(mPinHeightReceive[i * lineSize + j]);
            
            differenceHeight[i][j] =  output - input;

            
        }
    }
    
    // determine if each pin were touched or not
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if(flat[i][j]){
                if (abs(differenceHeight[i][j]) > 21) {
                    isTouched[i][j] = true;
                } else {
                    isTouched[i][j] = false;
                }
                
                
            } else {
                isTouched[i][j] = false;
            }
        }
    }
    
    //*** MODE: Every Pin Input ***//
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ i] = HIGH_THRESHOLD;
            
        }
    }
    
    
    int rangeDef = 10; //range of deformation
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                for (int ii = MAX(0,i - rangeDef); ii< MIN(RELIEF_SIZE_X,i+rangeDef); ii++) {
                    for (int jj = MAX(0,j - rangeDef); jj< MIN(RELIEF_SIZE_Y,j+rangeDef); jj++) {
                    int d = ofDist(i, j, ii, jj);
                    if(d>rangeDef){ d = rangeDef; };
                    int dHeight = ofMap(d, 0, rangeDef, (int)h, HIGH_THRESHOLD);
                    dHeight = MAX(LOW_THRESHOLD, dHeight);
                    allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ (ii)] =  MIN(allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ (ii)],dHeight);
                    }
                }
            }
            
        }
    }
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                h = MIN((int)h+35,HIGH_THRESHOLD);
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ (i)] = HIGH_THRESHOLD; //(int)h;
                for(int k = 0; k < filterFrame; k++){
                    allPixels_store[RELIEF_PHYSICAL_SIZE_X* j+ (i)][k] = HIGH_THRESHOLD; //(int)h;
                }
            }
        }
    }
    
    
    
    //*** MODE: One Center Pin Input ***//
//    int x = RELIEF_SIZE_X / 2;
//    int y = RELIEF_SIZE_Y / 2;
//    unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[x * lineSize + y]);
//    
//    int XShift = xCoordinateShift(x);
//    
//    for (int i = 0; i < RELIEF_PHYSICAL_SIZE_X; i++) {
//        for(int j = 0; j < RELIEF_PHYSICAL_SIZE_Y; j++){
//            
//            int d = ofDist(XShift, y, i, j);
//            if(d>15){ d = 15; };
//            int dHeight = ofMap(d, 0, 15, (int)h, HIGH_THRESHOLD);
//            dHeight = MAX(LOW_THRESHOLD, dHeight);
//            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ i] =  dHeight;
//        }
//    }
//    
//    allPixels[RELIEF_PHYSICAL_SIZE_X*y+XShift]=HIGH_THRESHOLD;
    
    
    
}

//----------------------------------------------------

void TouchShapeObject::renderShape()
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void TouchShapeObject::renderTangibleShape(int w, int h)
{
    ofSetColor((int) cpHeight);
    ofCircle(w/2, h/2, w/2);

    ofSetColor((int) cpHeight + 50);
    ofCircle(w/2, h/2, w/20);

    return;

    mOutputShapeImage.draw(0, 0, w, h);
    //ofFbo pinHeightMapImage; //FBO where we render height map
    //pinHeightMapImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    for (int x = 0; x < RELIEF_PHYSICAL_SIZE_X; x++) {
        for(int y = 0; y < RELIEF_PHYSICAL_SIZE_Y; y++){
            int distance = ofDist(RELIEF_PHYSICAL_SIZE_X/2, RELIEF_PHYSICAL_SIZE_Y/2, x, y);
            distance = min(15, distance);
            //int outputHeight = ofMap(distance, 0, 15, (int) controlPinHeight, 230);
            allPixels[RELIEF_PHYSICAL_SIZE_X * y + x] = (x + y) * 10 % 128;
        }
    }
}

//----------------------------------------------------

void TouchShapeObject::renderGraphics(int x, int y, int w, int h)
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------


void TouchShapeObject::drawGuiScreen(int x, int y, int w, int h)
{
    
    //Luke: todo: Must be drawn in CH viewport
    //            Remove GUI Translation for islands (not relevant for code)
    
    int pixelSize = 5;
    
    ofPushMatrix();
    ofTranslate(630, 0);
    ofFill();
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        
        
        
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            int val = differenceHeight[i][j];
            if(val< 0){
                ofSetColor(ofMap(val, 0, -160, 0, 255),0,0);
            } else {
                ofSetColor(0,ofMap(val, 0, 160, 0, 255),0);
            }
            
            ofRect(i*pixelSize,j*pixelSize,pixelSize,pixelSize);
            if (isTouched[i][j]) {
                ofNoFill();
                ofSetColor(255, 0, 0);
                ofRect(i*pixelSize,j*pixelSize,pixelSize,pixelSize);
                ofFill();
            }
            
            
            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+i]);
            
            ofSetColor(0,0,ofMap(output, 0, 160, 0, 255));
            ofRect(i*pixelSize +270,j*pixelSize,pixelSize,pixelSize);
        }
    }
    
    ofTranslate(0, 130);
    for (int i = 0; i < RELIEF_PHYSICAL_SIZE_X; i++) {
        for(int j = 0; j < RELIEF_PHYSICAL_SIZE_Y; j++){
            
            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+i]);
            ofSetColor(0,0,ofMap(output, 0, 160, 0, 255));
            ofRect(i*pixelSize,j*pixelSize,pixelSize,pixelSize);
        }
    }
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofRect(PINBLOCK_0_X_OFFSET*pixelSize, 0, PINBLOCK_0_WIDTH*pixelSize, RELIEF_SIZE_Y*pixelSize);
    ofRect(PINBLOCK_1_X_OFFSET*pixelSize, 0, PINBLOCK_1_WIDTH*pixelSize, RELIEF_SIZE_Y*pixelSize);
    ofRect(PINBLOCK_2_X_OFFSET*pixelSize, 0, PINBLOCK_2_WIDTH*pixelSize, RELIEF_SIZE_Y*pixelSize);
    
    ofPopMatrix();
    
}

//----------------------------------------------------

unsigned char* TouchShapeObject::getPixels()
{
    return allPixels;
}
/*
int TouchShapeObject::xCoordinateShift (int num){
    int val = num;
    if (num<16) {
        val =  PINBLOCK_0_X_OFFSET +num ;
    } else if (num<32){
        val = PINBLOCK_1_X_OFFSET +num -16 ;
    } else {
        val = PINBLOCK_2_X_OFFSET +num  -32;
    }
    return val;
}
*/