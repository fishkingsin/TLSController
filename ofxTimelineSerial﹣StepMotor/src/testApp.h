#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxUI.h"
#include "ofxMidi.h"

#define NUM_DEVICE 9
#define NUM_BYTES 1024

class testApp : public ofBaseApp , public ofxMidiListener {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxTimeline timeline;
	
    void receivedTrigger(ofxTLTriggerEventArgs& trigger);
    
    ofSerial serial;    
    bool		bSendSerialMessage;			// a flag for sending serial
    unsigned char		bytesRead[NUM_BYTES];				// data from serial, we will be trying to read 3
    char		bytesReadString[NUM_BYTES];			// a string needs a null terminator, so we need 3 + 1 bytes
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;					// when did we last read?				
    
    float maxStep,minStep;
    
    
    ofxUICanvas *gui,*gui1; 
    ofxUIToggleMatrix *matrix;
    vector <ofxUISlider*> sliders;
    ofxUIRadio *radio;
    ofxUIRangeSlider *range;
    int currentMotorPosition[NUM_DEVICE];
    void guiEvent(ofxUIEventArgs &e);
    
    //ofxMidi
    void newMidiMessage(ofxMidiMessage& eventArgs);
	
	stringstream text;
	
	ofxMidiIn	midiIn;
	ofxMidiMessage midiMessage;
};
