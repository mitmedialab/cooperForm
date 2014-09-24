//
//  UITriggers.h
//  cooperFORM
//
//  Created by Tangible Media Group on 9/24/14.
//
//

#ifndef __cooperFORM__UITriggers__
#define __cooperFORM__UITriggers__

#include "ReliefApplication.h"

class UITriggers {
public:

    
    // the button trigger event
    static void buttonTrigger(string name);
    
    // be sure to register relief application
    // so we can do stuff with it after events are triggered
    static void registerReliefApplication(ReliefApplication* relief);
    
private:
    // reference to the relief application
    // so we can actually do stuff when triggered
    static ReliefApplication *reliefApplication;
};

#endif /* defined(__cooperFORM__UITriggers__) */
