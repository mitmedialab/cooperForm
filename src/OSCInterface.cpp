//
//  OSCInterface.cpp
//  cooperFORM
//
//  Created by Tangible Media Group on 10/14/14.
//
//

#include "OSCInterface.h"
OSCInterface::OSCInterface(string host, int port) {
    this->host = host;
    this->port = port;
    sender.setup(host, port);
    cout << "connected" << endl;
}

void OSCInterface::sendModeChange(string newMode) {
    
    ofxOscMessage message;
    message.setAddress("/inform/mode");
    message.addStringArg(newMode);
    try {
        sender.sendMessage(message);
    } catch (...) {
        return;
    }
    
}