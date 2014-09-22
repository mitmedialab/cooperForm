//
//  VideoGrabber.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 9/22/14.
//
//

#include "VideoGrabber.h"


/* Video Grabber helper class */
VideoGrabber::VideoGrabber(int deviceID) {
    vector<ofVideoDevice> devices = listDevices();
    
    this->deviceID = deviceID;
    
    // find the device name
    for(int i = 0; i < devices.size(); i++){
        if (devices[i].id == deviceID) {
            deviceName = devices[i].deviceName;
            return;
        }
    }
    
    // device was never found
    throw "Device was not found!";
}
VideoGrabber::~VideoGrabber() {
    close();
}
int VideoGrabber::getDeviceID() {
    return deviceID;
}
string VideoGrabber::getDeviceName() {
    return deviceName;
}
