//
//  OSCInterface.h
//  cooperFORM
//
//  Created by Tangible Media Group on 10/14/14.
//
//

#ifndef __cooperFORM__OSCInterface__
#define __cooperFORM__OSCInterface__

#include <iostream>
#include "ofxOSC.h"

// our interface for communicating with the
// mac connected to the back display
class OSCInterface {
public:
    OSCInterface(string host, int port);
    
    // inform the other computer that
    // there's been a mode change
    void sendModeChange(string newMode);
    
private:
    string host;
    int port;
    ofxOscSender sender;
};

#endif /* defined(__cooperFORM__OSCInterface__) */
