#include "ofAppGlutWindow.h"
#include "ReliefApplication.h"
#include "UITriggers.h"
//#include "ofGLProgrammableRenderer.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    ofSetupOpenGL(&window, 2*1920, 1080, OF_FULLSCREEN);
    //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    //ofSetupOpenGL(2*1920, 1080, OF_WINDOW);
	
    ReliefApplication *relief = new ReliefApplication();    
    
    // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(relief);

}
