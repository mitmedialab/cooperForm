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
    
    grabbers = vector<VideoGrabber*>();

    vector<ofVideoDevice> devices = grabber.listDevices();
    cout << "Devices ..." << endl;
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName << endl;
        if( devices[i].bAvailable ){
            VideoGrabber* grabberForDevice = new VideoGrabber(i);
            grabberForDevice->initGrabber(320, 210);
            grabberForDevice->setDesiredFrameRate(60);
            grabbers.push_back(grabberForDevice);
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    cout << "End devices" << endl;
}

void CameraTracker::drawCameraFeed(string name, int x, int y, int w, int h) {
    VideoGrabber* grabber = getGrabberByName(name);
    //grabber->setDeviceID(grabber->getDeviceID());
    
    // ensure that grabber isn't null
    if (!grabber || grabber->getDeviceName() != name) {
        cout << "Camera wasn't found" << endl;
        return;
    }

    if (!grabber->isInitialized()) {
        grabber->initGrabber(w,h);
    }
    
    grabber->update();

    grabber->draw(x, y, w, h);
    
    //    ofImage camImage;
//    camImage.setUseTexture(false);
//   camImage.setFromPixels(grabber->getPixels(), grabber->getWidth(), grabber->getHeight(), OF_IMAGE_COLOR);
    
    //    camImage.draw(x,y , w,h);
    
}

VideoGrabber* CameraTracker::getGrabberByName(string name) {
    for (int i = 0; i < grabbers.size(); i++) {
        VideoGrabber* grabber = grabbers[i];
        if (grabber->getDeviceName() == name)
            return grabber;
    }
    return;
}

