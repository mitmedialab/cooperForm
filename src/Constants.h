/*
 *  Constants.h
 *  Relief
 *
 *  Created by Daniel Leithinger on 2/2014
 *  Copyright 2014 MIT Media Lab. All rights reserved.
 *
 */

#ifndef _CONSTANTS
#define _CONSTANTS	

#define	RELIEF_SIZE_X	  24
#define	RELIEF_SIZE_Y	  24

// clips values
// we get weird behavior when LOW_THRESHOLD is set to 0...
#define LOW_THRESHOLD 60
#define HIGH_THRESHOLD 245

// surface "pixels"
#define	RELIEF_PHYSICAL_SIZE_X	  24
#define	RELIEF_PHYSICAL_SIZE_Y	  24

#define RELIEF_SIZE 24 * 24

// multiple of the physical size
#define RELIEF_PROJECTOR_SIZE_X 480
#define RELIEF_PROJECTOR_SIZE_Y 480

#define KINECT_X 640
#define KINECT_Y 480

#define TOUCHSCREEN_SIZE_X      1920
#define TOUCHSCREEN_SIZE_Y      1080

#define MARGIN_X 420
#define TOUCHSCREEN_VISIBLE_SIZE_X TOUCHSCREEN_SIZE_X - (2 * MARGIN_X)

#define VERTICAL_DISPLAY_SIZE_X 1080
#define VERTICAL_DISPLAY_SIZE_Y 1920

#define PINBLOCK_0_X_OFFSET 13
#define PINBLOCK_0_WIDTH 16

#define PINBLOCK_1_X_OFFSET 43
#define PINBLOCK_1_WIDTH 16

#define PINBLOCK_2_X_OFFSET 73
#define PINBLOCK_2_WIDTH 16

#define KINECT_X 640
#define KINECT_Y 480

#define RELIEF_PROJECTOR_OFFSET_X 369
#define RELIEF_PROJECTOR_OFFSET_Y 285
#define RELIEF_PROJECTOR_SCALED_SIZE_X 607
#define RELIEF_PROJECTOR_SCALED_SIZE_Y 607

#define TOPVIEW_SCALE_X 33.0f
#define TOPVIEW_SCALE_Y 33.0f
#define TOPVIEW_SCALE_Z 33.0f

#define TERM_ID_HEIGHT (unsigned char)246
#define TERM_ID_HEIGHT_FEEDBACK (unsigned char)254
#define TERM_ID_GAIN_P (unsigned char)247
#define TERM_ID_GAIN_I (unsigned char)248
#define TERM_ID_MAX_I (unsigned char)249
#define TERM_ID_DEADZONE (unsigned char)250
#define TERM_ID_GRAVITYCOMP (unsigned char)251
#define TERM_ID_MAXSPEED (unsigned char)252

#define NUM_SERIAL_CONNECTIONS 3
#define NUM_ARDUINOS 96
#define NUM_PINS_ARDUINO 6

#define SERIAL_PORT_0 "/dev/tty.usbserial-A30010PW"
#define SERIAL_PORT_1 "/dev/tty.usbserial-A702YLM3"
#define SERIAL_PORT_2 "/dev/tty.usbserial-A702YMNY"
#define SERIAL_BAUD_RATE 115200

#define PID_RESET_INTERVALL 1500

//#define ARDUINO_GAIN_P 150
//#define ARDUINO_GAIN_I 35
//#define ARDUINO_MAX_I  60
//#define ARDUINO_DEADZONE 0

#define PROJECTOR_RAW_RESOLUTION_X 1400
#define PROJECTOR_RAW_RESOLUTION_Y 1050
#define SEAN_SCREEN_RESOLUTION_X 1680
#define SEAN_SCREEN_RESOLUTION_Y 1050
#define DANIEL_SCREEN_RESOLUTION_X 1440
#define DANIEL_SCREEN_RESOLUTION_Y 1050
#define CINEMA_DISPLAY_OFFSET 880
#define CINEMA_DISPLAY 0

#define SEAN_SCREEN 1

#endif