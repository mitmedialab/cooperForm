//
//  MoldShapeObject.cpp
//  basicExample
//
//  Created by Daniel Windham on 6/22/15.
//
//

#include "MoldShapeObject.h"


MoldShapeObject::MoldShapeObject()
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

MoldShapeObject::~MoldShapeObject() {
    moldedShapes.erase(moldedShapes.begin());
}

void MoldShapeObject::setup()
{
    
}

//----------------------------------------------------

void MoldShapeObject::update() {
    update(0.1);
}

//----------------------------------------------------

void MoldShapeObject::update(float dt)
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
        int XShift =  xCoordinateShift(i);

        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            
            int maxVal=0, minVal=255;
            for(int k =0; k < filterFrame; k++){
                maxVal = MAX(maxVal,(int)allPixels_store[RELIEF_PHYSICAL_SIZE_X*j+XShift][k]);
                minVal = MIN(minVal,(int)allPixels_store[RELIEF_PHYSICAL_SIZE_X*j+XShift][k]);
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
            int XShift =  xCoordinateShift(i);;

            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+XShift]);
            int input = int(mPinHeightReceive[i * lineSize + j]);
            
            differenceHeight[i][j] =  output - input;

            
        }
    }

    Boolean someoneIsTouched = false;
    int minXTouched = RELIEF_SIZE_X;
    int minYTouched = RELIEF_SIZE_Y;

    // determine if each pin were touched or not
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if(flat[i][j]){
                if (abs(differenceHeight[i][j]) > 21) {
                    isTouched[i][j] = true;
                    someoneIsTouched = true;
                    minXTouched = min(minXTouched, i);
                    minYTouched = min(minYTouched, j);
                } else {
                    isTouched[i][j] = false;
                }
                
                
            } else {
                isTouched[i][j] = false;
            }
        }
    }
    
    // determine what object resides in each pin
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            // initialize all values to -1
            holdsObject[i][j] = -1;
        }
    }

    if (someoneIsTouched) {
        // record new shape
        if (isRecording) {
            cout << endl << "is recording" << endl << endl;
            int depression[MOLDED_SHAPE_DIM][MOLDED_SHAPE_DIM];
            for (int i = 0; i < MOLDED_SHAPE_DIM; i++) {
                for (int j = 0; j < MOLDED_SHAPE_DIM; j++) {
                    int x = i + minXTouched;
                    int y = j + minYTouched;
                    if (x < RELIEF_SIZE_X && y < RELIEF_SIZE_Y && isTouched[x][y]) {
                        depression[i][j] = differenceHeight[x][y];
                    } else {
                        depression[i][j] = 0;
                    }
                }
            }
            
            MoldedShape *newShape = new MoldedShape(getUID(), depression);
            newShape->x = minXTouched;
            newShape->y = minYTouched;
            moldedShapes.push_back(newShape);
        }
    }
    
    //*** MODE: Every Pin Input ***//

    int midHeight = (LOW_THRESHOLD + HIGH_THRESHOLD) / 2;
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = midHeight;
        }
    }

    // draw molded shapes into allPixels array
    for (vector<MoldedShape *>::iterator itr = moldedShapes.begin(); itr != moldedShapes.end(); itr++) {
        for (int i = 0; i < MOLDED_SHAPE_DIM; i++) {
            int x = i + (*itr)->x;
            if (x >= RELIEF_SIZE_X) { break; }
            for (int j = 0; j < MOLDED_SHAPE_DIM; j++) {
                int y = j + (*itr)->y;
                if (y >= RELIEF_SIZE_Y) { break; }
                int shapeHeight = (*itr)->heightMap[i][j];
                int proposedHeight = min(midHeight + shapeHeight, HIGH_THRESHOLD);
                int index = RELIEF_PHYSICAL_SIZE_X * y + xCoordinateShift(x);
                if (proposedHeight > (int) allPixels[index]) {
                    allPixels[index] = proposedHeight;
                }
            }
        }
    }

    // clear isRecording flag
    isRecording = false;
}

//----------------------------------------------------

void MoldShapeObject::renderShape()
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void MoldShapeObject::renderTangibleShape(int w, int h)
{
    float xStep = (float) w / RELIEF_PHYSICAL_SIZE_X;
    float yStep = (float) h / RELIEF_PHYSICAL_SIZE_Y;
    for (int x = 0; x < RELIEF_PHYSICAL_SIZE_X; x++) {
        for (int y = 0; y < RELIEF_PHYSICAL_SIZE_Y; y++) {
            ofSetColor(allPixels[RELIEF_PHYSICAL_SIZE_X * y + x]);
            ofRect(x * xStep, y * yStep, xStep, yStep);
        }
    }
}

//----------------------------------------------------

void MoldShapeObject::renderGraphics(int x, int y, int w, int h)
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void MoldShapeObject::drawGuiScreen(int x, int y, int w, int h)
{
    int pixelSize = 5;
    
    ofPushMatrix();
    ofTranslate(630, 0);
    ofFill();
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        if(i==16 || i == 32){
            ofTranslate(5, 0);
        }
        
        int XShift = xCoordinateShift(i);
        
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
            
            
            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+XShift]);
            
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

void MoldShapeObject::setTableValuesForShape(ShapeIOManager *pIOManager)
{
    if (false && isRecording) {
        pIOManager->set_max_speed(0);
        pIOManager->set_gain_p(1.5f);
        pIOManager->set_gain_i(0.045f);
        pIOManager->set_max_i(25);
        pIOManager->set_deadzone(2);
    } else {
        pIOManager->set_max_speed(200);
        pIOManager->set_gain_p(1.5f);
        pIOManager->set_gain_i(0.045f);
        pIOManager->set_max_i(25);
        pIOManager->set_deadzone(2);
    }
};

//----------------------------------------------------



unsigned char* MoldShapeObject::getPixels()
{
    return allPixels;
}

int MoldShapeObject::xCoordinateShift (int num){
    return num;

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