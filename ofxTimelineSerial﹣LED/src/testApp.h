#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxMidi.h"
#define NUM_DEVICE 3
#define NUM_BYTES 256
class testApp : public ofBaseApp, public ofxMidiListener {

	public:

		void setup();
		void update();
		void draw();
        //void threadedFunction();
        void exit()
        {
//            for(int i = 0 ; i < NUM_DEVICE ; i++)
//            {
//                string mystr = ofToString(i+1,0)+",0,0,0,0\n";
//                
//                serial.writeBytes((unsigned char*)mystr.c_str(),mystr.length());
//            }
            //stopThread();
        }

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
    unsigned char		bytesReadString[NUM_BYTES];			// a string needs a null terminator, so we need 3 + 1 bytes
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;					// when did we last read?				
    
    //ofxMidi
    void newMidiMessage(ofxMidiMessage& eventArgs);
	
	stringstream text;
	
	ofxMidiIn	midiIn;
	ofxMidiMessage midiMessage;

};
