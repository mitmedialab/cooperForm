//
//  CameraTracker.cpp
//  cooperFORM
//
//  Created by Jared Counts on 9/22/14.
//
//

#include "CameraTracker.h"

void CameraTracker::setup() {
    ofVideoGrabber grabber = ofVideoGrabber();
    
    grabbers = vector<ofVideoGrabber>();

    vector<ofVideoDevice> devices = grabber.listDevices();
    cout << "Devices ..." << endl;
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName << endl;
        if( devices[i].bAvailable ){
            ofVideoGrabber grabberForDevice = ofVideoGrabber();
            grabberForDevice.setDeviceID(devices[i].id);
            grabberForDevice.initGrabber(1920, 1080);
            grabbers.push_back(grabberForDevice);
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    cout << "End devices" << endl;

}

void CameraTracker::drawCameraFeed(int id, int x, int y, int w, int h) {
    ofVideoGrabber grabber = grabbers[id];

    if (!grabber.isInitialized())
        grabber.initGrabber(320,210);
    
    grabber.update();

    grabber.draw(x, y, w, h);
    
}

CameraTracker::~CameraTracker() {
    for (ofVideoGrabber grabber : grabbers)
        grabber.close();
    
    grabbers.clear();
}
