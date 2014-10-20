//
//  KinectTracker.cpp
//  basicExample
//
//  Created by Tangible Media Group on 2/16/14.
//
//


#include "KinectTracker.h"

void KinectTracker::setup(int pNearThreshold, int pFarThreshold, int pContourMinimumSize) {
	
    if(kinect.numAvailableDevices() > 0) {
        kinect.setRegistration(true); // enable depth->video image calibration
        kinect.init();
        kinect.open();
    }
    
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
	colorImg.allocate(kinect.width, kinect.height);
    depthImg.allocate(kinect.width, kinect.height);
    
    colorImgCropped.allocate(kinect.width, kinect.height);
    depthImgCropped.allocate(kinect.width, kinect.height);
    
//	grayThreshNear.allocate(kinect.width, kinect.height);
//	grayThreshFar.allocate(kinect.width, kinect.height);
	depthThreshed.allocate(kinect.width, kinect.height);
    lastDepthThreshed.allocate(kinect.width, kinect.height);
    depthThreshedDiff.allocate(kinect.width, kinect.height);
    recordingImage.allocate(kinect.width*2, kinect.height, OF_IMAGE_COLOR);
    playingImage.allocate(kinect.width*2, kinect.height, OF_IMAGE_COLOR);
    
	mNearThreshold = pNearThreshold;
	mFarThreshold = pFarThreshold;
    mContourMinimumSize = pContourMinimumSize;

    
    loadAlphaMaskAndPrepForCvProcessing();
    
    cropX = 0;
    cropY = 0;
    cropWidth = kinect.width;
    cropHeight = kinect.height;
}

//--------------------------------------------------------------
//
// Update
//
// If playing from a recording, we set
// the colorImg and depthImg from the video
// and the rest of the class continues to reference
// colorImg and depthImg, rather then needing to
// reference different images depending on the input.
//
//--------------------------------------------------------------

void KinectTracker::update() {
    
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
        // update from kinect
        updateImagesFromKinect();
        
        depthImg.dilate_3x3();
        depthImg.erode_3x3();
        
        cropImages();
        
        lastDepthThreshed.setFromPixels(depthThreshed.getPixels(), depthThreshed.getWidth(), depthThreshed.getHeight());
        // always update the depth image
        depthThreshed.setFromPixels(depthImgCropped.getPixels(), depthImgCropped.getWidth(), depthImgCropped.getHeight());
        colorImgThreshed.setFromPixels(colorImgCropped.getPixels(), colorImgCropped.getWidth(), colorImgCropped.getHeight());
        
        
        // subtract mask which is png alpha image called "mask.png"
        if(useMask) subtractMask();
                
        // threshold calcutations convery depth map into black and white images
		calculateThresholdsAndModifyImages();
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// if find holes is set to true, we will get interior contours as well.
		//contourFinder.findContours(depthImg, mContourMinimumSize, (kinect.width*kinect.height)/2, 10, false);
        
        depthThreshedDiff.absDiff(lastDepthThreshed, depthThreshed);
	}
}

// we are using kinect
void KinectTracker::updateImagesFromKinect() {
    colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
    depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
    
    colorImg.mirror(true,true);
    depthImg.mirror(true,true);
}

// used by the player class to set the current frame from now playing movie
void KinectTracker::updateCurrentFrame(unsigned char * pixels, int w, int h) {
    playingImage.setFromPixels(pixels, w, h, OF_IMAGE_COLOR);
}

void KinectTracker::flagImagesAsChanged() {
    colorImg.flagImageChanged();
    depthImg.flagImageChanged();
    depthThreshed.flagImageChanged();
}

void KinectTracker::cropImages() {
//    if (cropX == 0 && cropY == 0 && cropWidth == colorImg.getWidth() && cropHeight == colorImg.getHeight())
//        return;
    
    ofImage fullColor;
    fullColor.allocate(colorImg.getWidth(), colorImg.getHeight(), OF_IMAGE_COLOR);
    fullColor.setFromPixels(colorImg.getPixels(), colorImg.getWidth(), colorImg.getHeight(), OF_IMAGE_COLOR);
    fullColor.crop(cropX, cropY, cropWidth, cropHeight);
    colorImgCropped.setFromPixels(fullColor.getPixels(), fullColor.getWidth(), fullColor.getHeight());

    ofImage fullDepth;
    fullDepth.allocate(depthImg.getWidth(), depthImg.getHeight(), OF_IMAGE_GRAYSCALE);
    fullDepth.setFromPixels(depthImg.getPixels(), fullDepth.getWidth(), fullDepth.getHeight(), OF_IMAGE_GRAYSCALE);
    fullDepth.crop(cropX, cropY, cropWidth, cropHeight);
    depthImgCropped.setFromPixels(fullDepth.getPixels(), fullDepth.getWidth(), fullDepth.getHeight());
}

void KinectTracker::subtractMask() {
    cvAnd(depthImg.getCvImage(), maskCv.getCvImage(), depthImg.getCvImage(), NULL);
    //cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), depthImg.getCvImage(), NULL);
}

// loads png mask and converts to cv grayscale which we need to cvAnd method
void KinectTracker::loadAlphaMaskAndPrepForCvProcessing() {
    
    // type is OF_IMAGE_COLOR_ALPHA
    mask.loadImage("mask.png");
    
    // simple way to convert to differe image type,
    // changing the transparent areas to white
    ofImage image;
    image.setFromPixels(mask.getPixelsRef());
    image.setImageType(OF_IMAGE_COLOR);
    maskColorCv.setFromPixels(image.getPixels(), mask.getWidth(), mask.getHeight());
    maskCv = maskColorCv;
    
}

void KinectTracker::calculateThresholdsAndModifyImages() {
//    depthImg.erode_3x3();
//    depthImg.dilate_3x3();
    
    // we do two thresholds - one for the far plane and one for the near plane
    // we then do a cvAnd to get the pixels which are a union of the two thresholds
//    grayThreshNear = depthImg;
//    grayThreshFar = depthImg;
//    grayThreshNear.threshold(mNearThreshold, true);
//    grayThreshFar.threshold(mFarThreshold);
//    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), depthImg.getCvImage(), NULL);
    
    // find depth map excluding thresholded data
    // this causes the 10 finger effect and could be related to our discussion
    // today about dynamic thresholding
    //
    // if we threshold with the near value, and the user moves the hand just past the near point
    // and thus out of range
    // their hand will be black (since black is used for out of range areas)
    // however since their hands shadow is also black this will cause the 10 finger effect.
    //
    //cvAnd(grayThreshNear.getCvImage(), depthThreshed.getCvImage(), depthThreshed.getCvImage(), NULL);
//    cvAnd(grayThreshFar.getCvImage(), depthThreshed.getCvImage(), depthThreshed.getCvImage(), NULL);
    
//    depthThreshed = depthImgCropped;
    // remap pixels so they range from 0-255 after thresholding
    ofPixelsRef depthPixels = depthThreshed.getPixelsRef();
    ofPixelsRef colorPixels = colorImgThreshed.getPixelsRef();
    for (int x = 0; x < depthPixels.getWidth(); x++) {
        for (int y = 0; y < depthPixels.getHeight(); y++) {
            float shade = ofClamp((depthPixels.getColor(x,y).getBrightness() - mFarThreshold) * 255.f / (mNearThreshold - mFarThreshold), 0, 255);
            
            depthPixels.setColor(x,y, shade);
        }
    }
    
    for (int x = 1; x < depthPixels.getWidth()-1; x++) {
        for (int y = 1; y < depthPixels.getHeight()-1; y++) {
                if (depthPixels.getColor(x, y).getBrightness() < 40)
                    colorPixels.setColor(x,y, 0);
                if (depthPixels.getColor(x-1, y).getBrightness() < 20)
                    colorPixels.setColor(x,y, 0);
                if (depthPixels.getColor(x, y-1).getBrightness() < 20)
                    colorPixels.setColor(x,y, 0);
                if (depthPixels.getColor(x+1, y).getBrightness() < 20)
                    colorPixels.setColor(x,y, 0);
                if (depthPixels.getColor(x, y+1).getBrightness() < 20)
                    colorPixels.setColor(x,y, 0);
        }
    }
    depthThreshed.flagImageChanged();
    colorImgThreshed.flagImageChanged();
}

//--------------------------------------------------------------
//
// Draw various images
//
//--------------------------------------------------------------

// color image
void KinectTracker::drawColorImage(int x, int y, int width, int height) {
    ofSetColor(255);
    colorImg.draw(x, y, width, height);
}

// color image
void KinectTracker::drawColorThresholdImage(int x, int y, int width, int height) {
    ofSetColor(255);
    colorImgThreshed.draw(x, y, width, height);
}

// gray image with contour blobs drawn on top
void KinectTracker::drawThresholdImage(int x, int y, int width, int height){
    ofSetColor(255);
    depthThreshed.draw(x,y,width,height);
    //depthImg.draw(x, y, width, height);
    //contourFinder.draw(x, y, width, height);
}

// draw from the live kinect
void KinectTracker::drawDepthImage(int x, int y, int width, int height){
    ofSetColor(255);
    depthImg.draw(x, y, width, height);
}

// black and white image from within threshold range
void KinectTracker::drawDepthThreshedImage(int x, int y, int width, int height) {
    ofSetColor(255);
    depthThreshed.draw(x, y, width, height);
}

// black and white difference image from within threshold range
// (only data from movements)
void KinectTracker::drawDepthThreshedDiff(int x, int y, int width, int height) {
    ofSetColor(255);
    depthThreshedDiff.draw(x,y, width, height);
}


//--------------------------------------------------------------
//
// Get various kinect images as pixels
//
//--------------------------------------------------------------

// standard color image from kinect video camera
unsigned char * KinectTracker::getColorPixels() {
    return colorImg.getPixels();
}

// kinect depth map
// grayscale image where white = near, black = far
unsigned char * KinectTracker::getDepthPixels() {
    return kinect.getDepthPixels();
}

// kinect depth map WHERE
// areas outside near and far threshold range are black.
// This is currently used by the wave animation
unsigned char * KinectTracker::depthThreshedPixels() {
    return depthThreshed.getPixels();
}

// kinect depth map WHERE
// areas outside near and far threshold range are black.
// This is currently used by the wave animation
ofPixels KinectTracker::depthThresholdOFPixels() {
    return depthThreshed.getPixelsRef();
}


// kinect depth map WHERE
// only black OR white (0 OR 255) is used.
// calculated using using pContourMinimumSize
unsigned char * KinectTracker::grayImagePixels() {
    return depthImg.getPixels();
}

// returns pixels
// color image is on left and depth is on right
unsigned char * KinectTracker::getRecordingPixels() {
    return recordingImage.getPixels();
}


// get a reference to the contour finder
ofxCvContourFinder * KinectTracker::getContourFinder() {
    return &contourFinder;
}


//--------------------------------------------------------------
//
// Kinect pass through getters
//
// Since we wrap the kinect class we no longer have access to
// its methods unless we add them to KinectTracker too.
//
//--------------------------------------------------------------
int KinectTracker::numAvailableDevices(){
    return kinect.numAvailableDevices();
}

bool KinectTracker::isFrameNew() {
    return kinect.isFrameNew();
}

bool KinectTracker::isConnected() {
    return kinect.isConnected();
}

void KinectTracker::setCrop(int x, int y, int width, int height) {
    cropX = x;
    cropY = y;
    cropWidth = width;
    cropHeight = height;
    
    colorImgCropped.allocate(cropWidth, cropHeight);
    depthImgCropped.allocate(cropWidth, cropHeight);
    depthThreshed.allocate(cropWidth, cropHeight);
    lastDepthThreshed.allocate(cropWidth, cropHeight);
    depthThreshedDiff.allocate(cropWidth, cropHeight);
}