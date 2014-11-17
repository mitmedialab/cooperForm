//
//  CityShapeObject.cpp
//  cooperFORM
//
//  Created by ken_tmg on 11/7/14.
//
//

#include "CityShapeObject.h"
#include "Constants.h"

CityShapeObject::CityShapeObject() {
    touchscreenImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    pinHeightMapImage.allocate(TOUCHSCREEN_VISIBLE_SIZE_X, TOUCHSCREEN_SIZE_Y, GL_RGBA);
    projectorImage.allocate(PROJECTOR_RAW_RESOLUTION_X, PROJECTOR_RAW_RESOLUTION_Y, GL_RGBA);
    
    
    
    pos = ofVec2f(TOUCHSCREEN_SIZE_Y/2, TOUCHSCREEN_SIZE_Y/2);
    length = 200;
    rectLength = 200*300/TOUCHSCREEN_SIZE_Y ;
    
    
    cityMov[0][0].loadMovie("City/mov/Shadow.mov");
    cityMov[0][1].loadMovie("City/mov/Shadow_bw.mov");
    cityMov[1][0].loadMovie("City/mov/Traffic.mov");
    cityMov[1][1].loadMovie("City/mov/Traffic_bw.mov");
    cityMov[2][0].loadMovie("City/mov/Energy.mov");
    cityMov[2][1].loadMovie("City/mov/Energy_bw.mov");
    cityMov[3][0].loadMovie("City/mov/Sequence.mov");
    cityMov[3][1].loadMovie("City/mov/Sequence_bw.mov");
    
    realMap.loadImage("City/map.jpg");
    
    
    trimmedMov[0].allocate(rectLength, rectLength, GL_RGB);
    trimmedMov[1].allocate(rectLength, rectLength, GL_RGB);
    
    trimMov 	= new unsigned char[rectLength*rectLength*3];
    
    currentCity = 0;
    previousCity = 0;
    
    for(int i = 0; i < cityNum; i++){
        for(int j = 0; j < 2; j++){
            cityMov[i][j].play();
            cityMov[i][j].setSpeed(0.7);
        }
    }
    

    
    mapShadow.loadImage("City/assets/map-shadow.png");
    loupe.loadImage("City/assets/loupe.png");
}

//--------------------------------------------------------------

void CityShapeObject::update() {
    

    //for(int i = 0; i < cityNum; i++){
        for(int j = 0; j < 2; j++){
            if(sliderGrabbed){
            cityMov[currentCity][j].setPosition( time);
            }
            cityMov[currentCity][j].update();
        }
    //}
    posX = int(pos.x*300/TOUCHSCREEN_SIZE_Y);
    posY = int(pos.y*300/TOUCHSCREEN_SIZE_Y);
    
    
    if (cityMov[currentCity][0].isFrameNew() || loupeDragged){
    for (int i = 0; i < 2; i++){
        
    unsigned char * pixels = cityMov[currentCity][i].getPixels();
    for (int j = 0; j < rectLength; j++){ //x
        for (int k = 0; k < rectLength; k++) { //y
                        for(int l =0; l < 3; l++){ //rgb
            trimMov[j*3+rectLength*k*3 +l] =pixels[(j+posX-rectLength/2)*3 +300*(k+posY-rectLength/2)*3 + l];
            }
        }
    }
    trimmedMov[i].loadData(trimMov, rectLength,rectLength, GL_RGB);
        
    }
}
    
    
    
}

void CityShapeObject::reset() {
}

//--------------------------------------------------------------

void CityShapeObject::renderProjectorOverlay(int w, int h) {
    
    trimmedMov[0].draw(510, 242, 3151, 1778);
    
    
}

//--------------------------------------------------------------

void CityShapeObject::renderTangibleShape(int w, int h) {
    
    trimmedMov[1].draw(0, 0, w, h);
}

//--------------------------------------------------------------

void CityShapeObject::renderTouchscreenGraphics(int w, int h) {

    
    //cityMov[currentCity][0].draw(0,0,h,h);
    
    realMap.draw(0,0,h,h);
    mapShadow.draw(0,0, h, h);
    
    
    loupe.draw(pos.x - loupe.width/2, pos.y - loupe.height/2, loupe.width, loupe.height);
    ofNoFill();
    //ofRect(pos.x - length/2, pos.y - length/2, length, length);
    
    
    
    
    trimmedMov[0].draw(0,0);
    trimmedMov[1].draw(rectLength,0);
}

//--------------------------------------------------------------


void CityShapeObject::setMouseDragInfo(int x, int y, int button){
    x = x-420;
    if( loupeDragged){
        pos.x = pos.x - (oldMouse.x - x);
        pos.x = ofClamp(pos.x,length/2,TOUCHSCREEN_SIZE_Y-length/2);
        
        pos.y = pos.y - (oldMouse.y - y);
        
        pos.y = ofClamp(pos.y,length/2,TOUCHSCREEN_SIZE_Y-length/2);
        oldMouse= ofVec2f(x,y);
        
        
    }
    

}

//--------------------------------------------------------------

void CityShapeObject::setMousePressedInfo(int x, int y){
    x = x-420;
    if( abs(x - pos.x) < length/2 && abs(y - pos.y) < length/2){
        oldMouse = ofVec2f(x,y);
        loupeDragged = true;
    } else {
        loupeDragged = false;
    }
    
}

//--------------------------------------------------------------

void CityShapeObject::setMouseReleasedInfo(){
        loupeDragged = false;
    
}

//--------------------------------------------------------------

float CityShapeObject:: getMovPosition(){
    
    return cityMov[currentCity][0].getPosition();
}

void CityShapeObject:: setMovPosition(float t){
    
    time = t;

}

void CityShapeObject:: chooseCity(int num){
    
    float val = cityMov[currentCity][0].getPosition();
    currentCity = num;
    
    for (int j = 0; j < 2; j++){
        cityMov[currentCity][j].setPosition(val);
        cityMov[currentCity][j].update();
    }
}