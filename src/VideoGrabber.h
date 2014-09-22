//
//  VideoGrabber.h
//  cooperFORM
//
//  Created by Tangible Media Group on 9/22/14.
//
//

#ifndef __cooperFORM__VideoGrabber__
#define __cooperFORM__VideoGrabber__

#include <iostream>
#include "ofMain.h"

// wrapper for ofVideoGrabber
// because for some reason,
// ofVideoGrabber has a setDevice but no getDevice!!
class VideoGrabber : public ofVideoGrabber {
    
public:
    VideoGrabber(int deviceID);
    virtual ~VideoGrabber();
    int getDeviceID();
    string getDeviceName();
    
private:
    int deviceID;
    string deviceName;
};


#endif /* defined(__cooperFORM__VideoGrabber__) */
