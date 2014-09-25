#include "ofAppGlutWindow.h"
#include "ReliefApplication.h"
#include "UITriggers.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024, 768, OF_FULLSCREEN);
    
    
	
    ReliefApplication *relief = new ReliefApplication();
    
    // register
    UITriggers::registerReliefApplication(relief);
    
    
    // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(relief);

}
