#include "testApp.h"
#include "string.h"
int eachWidth=0;
int sendwhich = 0;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	glEnable(GL_DEPTH_TEST);
	//ofEnableLighting();
	eachWidth = ofGetScreenWidth()/NUM_DEVICE;
	timeline.setup();
    timeline.setWidth(ofGetScreenWidth());
	
	timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.enableSnapToBPM(120.f);
	timeline.enableDrawBPMGrid(true);
    waveform.setup();
	waveform.loadSoundfile("6)219 - Howlin' Wolf - Spoonful (full).wav");
    timeline.setDurationInSeconds(waveform.getDuration());
    timeline.addElement("Track", &waveform);	
	
    for(int i = 0 ; i < NUM_DEVICE ; i++)
    {
        
        string device_name = "Device_"+ofToString(i,0);
        //timeline.addPage(device_name+" Page");
        //timeline.addKeyframes(device_name+"_birghtness",    device_name+"birghtness.xml",   ofRange(0, 255));
        timeline.addKeyframes(device_name+"_Red",           device_name+"Red.xml",          ofRange(0, 255));
        timeline.addKeyframes(device_name+"_Green",          device_name+"Green.xml",        ofRange(0, 255));
        timeline.addKeyframes(device_name+"_Blue",           device_name+"Blue.xml",         ofRange(0, 255));
        
    }
    
	
	timeline.getColors().loadColors("defaultColors.xml");
    timeline.setCurrentPage(1);
	ofAddListener(ofxTLEvents.trigger, this, &testApp::receivedTrigger);
    
    
    //setup serial
    serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	//----------------------------------- note:
	// < this should be set
	// to whatever com port
	// your serial device is
	// connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
    serial.setup(deviceList[0].getDeviceName(), 57600);
    //serial.setup("/dev/tty.usbserial-A4001JgX", 57600);
	//serial.setup("/dev/tty.usbserial-11U3V4KS", 57600);
    serial.setVerbose(false);
    //startThread();
    //ofThread::verbose = false;
    //serial.setVerbose(false);
}

//--------------------------------------------------------------
//void testApp::threadedFunction()
//{
//    while( isThreadRunning() != 0 ){
//        //if( lock() ){
//            
//            //unlock();
//            //ofSleepMillis(10);
//        //}
//    }
//}
//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
}
//--------------------------------------------------------------

void testApp::update(){ 
    string device_name = "Device_"+ofToString(sendwhich);
    int r_  = ofMap(timeline.getKeyframeValue(device_name+"_Red"),0,255,1,254);
    int g_  = ofMap(timeline.getKeyframeValue(device_name+"_Green"),0,255,1,254);
    int b_  = ofMap(timeline.getKeyframeValue(device_name+"_Blue"),0,255,1,254);
    
    string mystr = ofToString(sendwhich+1,0)+","+ofToString(r_,0)+","+ofToString(g_,0)+","+ofToString(b_,0)+"\n";
    //ofLog(OF_LOG_VERBOSE,"Serial write bytes : "+mystr);
    serial.writeBytes((unsigned char*)mystr.c_str(),mystr.length());
    
    
    nTimesRead = 0;
    nBytesRead = 0;
    int nRead  = 0;  // a temp variable to keep count per read
    
    unsigned char bytesReturned[NUM_BYTES];
    
    memset(bytesReadString, 0, NUM_BYTES);
    memset(bytesReturned, 0, NUM_BYTES);
    
    while( (nRead = serial.readBytes( bytesReturned, NUM_BYTES)) > 0){
        nTimesRead++;	
        nBytesRead = nRead;
    };
    memcpy(bytesReadString, bytesReturned, NUM_BYTES);
    if(strlen((char*)bytesReadString)>0)printf("%s",(char*)bytesReadString);
    
    sendwhich++;
    sendwhich%=NUM_DEVICE;
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofEnableAlphaBlending();
    
    
    for(int i = 0 ; i < NUM_DEVICE ; i++)
    {
        
        string device_name = "Device_"+ofToString(i);
       // int a_ = timeline.getKeyframeValue(device_name+"_birghtness");
        int r_ = timeline.getKeyframeValue(device_name+"_Red");
        int g_ = timeline.getKeyframeValue(device_name+"_Green");
        int b_ = timeline.getKeyframeValue(device_name+"_Blue");
        //float bright = a_/255.0f;
        ofPushStyle();
        ofSetColor(r_ ,g_ ,b_ );
        ofRect(i*eachWidth, 0, eachWidth, eachWidth);
        ofPopStyle();
        ofPushStyle();
        ofSetColor(200, 200, 200);
        ofNoFill();
        ofRect(i*eachWidth, 0, eachWidth, eachWidth);
        ofPopStyle();
        
    }
    timeline.draw();
    //char str[256];
    //sprintf(str,"bytesReturned[0] %i,bytesReturned[1] %i,bytesReturned[2] %i, bytesReturned[3] %i\n",bytesReadString[0],bytesReadString[1],bytesReadString[2],bytesReadString[3]);
    
    //ofDrawBitmapString(str, ofGetWidth()/2,ofGetHeight()/2);
}
//-------------------------------------------------------------- 
void testApp::receivedTrigger(ofxTLTriggerEventArgs& trigger){
	if(trigger.triggerName == "RED"){
		
	}
	else if(trigger.triggerName == "GREEN"){
		
	}
	else if(trigger.triggerName == "BLUE"){
		
	}
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 27:
            for(int i = 0 ; i < NUM_DEVICE ; i++)
            {
                string mystr = ofToString(i+1,0)+","+ofToString(0,0)+","+ofToString(0,0)+","+ofToString(0,0)+"\n";
                ofLog(OF_LOG_VERBOSE,"Serial write bytes : "+mystr);
                serial.writeBytes((unsigned char*)mystr.c_str(),mystr.length());
            }
            break;
        case ' ':
            waveform.togglePlay();
            break;
//        case OF_KEY_RETURN:
//            timeline.togglePlay();
//            break;
        case 'h':
            timeline.toggleShow();
            break;
            case 'f':
            ofToggleFullscreen();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}