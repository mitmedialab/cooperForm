//
//  CameraTracker.h
//  cooperFORM
//
//  Created by Jared Counts on 9/22/14.
//
//

#ifndef __cooperFORM__CameraTracker__
#define __cooperFORM__CameraTracker__

#include <iostream>
#include "ofMain.h"
#include "VideoGrabber.h"

class CameraTracker {
    
public:
    void setup();
    void drawCameraFeed(string name, int x, int y, int w, int h);
    
private:
    vector<VideoGrabber*> grabbers;
    
    // Returns the first grabber that matches this name
    VideoGrabber* getGrabberByName(string name);
    
};

#endif /* defined(__cooperFORM__CameraTracker__) */
