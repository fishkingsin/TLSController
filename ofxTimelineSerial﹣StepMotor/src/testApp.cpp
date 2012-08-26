#include "testApp.h"
#include "string.h"
//int eachWidth=0;
//int sendwhich = 0;
int timeDiff = 0;
bool isSent = false;
//--------------------------------------------------------------
void testApp::setup(){
    ofxXmlSettings config;
    
    
    if(!config.loadFile("config.xml"))
    {
        printf("Cant Load config.xml");
    }
    switch (config.getValue("XML:LOG_LEVEL",5)) {
        case 5:
            ofSetLogLevel(OF_LOG_VERBOSE);
            break;
        case 4:
            ofSetLogLevel(OF_LOG_NOTICE);
            break;
        case 3:
            ofSetLogLevel(OF_LOG_WARNING);
            break;
        case 2:
            ofSetLogLevel(OF_LOG_ERROR);
            break;
        case 1:
            ofSetLogLevel(OF_LOG_FATAL_ERROR);
            break;
        case 0:
            ofSetLogLevel(OF_LOG_SILENT);
            break;
        default:
            
            ofSetLogLevel(OF_LOG_VERBOSE);
            
            break;
    }
    
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofBackground(125,125,125);
    ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	glEnable(GL_DEPTH_TEST);
	timeline.setup();
    timeline.setWidth(ofGetScreenWidth());
	timeline.setDurationInFrames(30*60*3);
	timeline.setLoopType(OF_LOOP_NORMAL);
    
	timeline.addTriggers("Step_0", "Step_0.xml");
    timeline.getColors().loadColors("defaultColors.xml");
    //    timeline.setCurrentPage(1);
    //    timeline.setSnapping(true);
    //    timeline.enableSnapToBPM(120.f);
    //    timeline.enableDrawBPMGrid(true);
    //    timeline.enableSnapToOtherElements(true);
    //    timeline.setMovePlayheadOnDrag(false);
    //    timeline.setMovePlayheadOnPaste(true);
	ofAddListener(ofxTLEvents.trigger, this, &testApp::receivedTrigger);
    
    //setup serial
    serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    float yInit = 500;
    float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
    
	gui = new ofxUICanvas(0,ofGetScreenHeight()-yInit,320,yInit);
    gui->addWidgetDown(new ofxUILabel("Setpper Motor Control Panel", OFX_UI_FONT_SMALL)); 
//    gui->addWidgetDown(new ofxUILabel("Press Space to Hide GUIs", OFX_UI_FONT_SMALL));
    vector<string> list;
    for(int i = 0 ; i<    deviceList.size() ; i++)
    {
        string s(deviceList[i].getDeviceName());
        list.push_back(s);
    }
    ofxUIDropDownList *dropList = new ofxUIDropDownList(200, "DEVICE_LIST", list, OFX_UI_FONT_SMALL);
    dropList->setAutoClose(true);
    gui->addWidgetDown(dropList);
    vector<string>log_level_list;
    log_level_list.push_back("LOG_VERBOSE");
    log_level_list.push_back("LOG_NOTICE");
    log_level_list.push_back("LOG_WARNING");
    log_level_list.push_back("LOG_ERROR");
    log_level_list.push_back("LOG_FATAL_ERROR");
    log_level_list.push_back("LOG_SILENT");
    gui->addWidgetDown(new ofxUIDropDownList(200, "LOG_LEVEL", log_level_list, OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_SMALL)); 
    gui->addWidgetDown(new ofxUIButton(32, 32, false, "SAVE"));
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    gui->addWidgetDown(new ofxUIButton(32, 32, false, "RESET"));
    gui->addWidgetDown(new ofxUIButton(32, 32, false, "SEND"));
    
    
    gui1 = new ofxUICanvas(320, ofGetScreenHeight()-yInit,ofGetWidth()-320,yInit); 
	gui1->addWidgetDown(new ofxUILabel("PANEL 1: Setpper Menual Control", OFX_UI_FONT_SMALL)); 
	gui1->addWidgetDown(new ofxUILabel("Step Range", OFX_UI_FONT_SMALL)); 
    gui1->addWidgetDown(new ofxUIButton(32, 32, false, "CLOCKWISE"));
    gui1->addWidgetRight(new ofxUIButton(32, 32, false, "COUNTER_CLOCKWISE"));
    range = new ofxUIRangeSlider(length-xInit,dim,
                                 config.getValue("XML:MIN_STEP",-3600),
                                 config.getValue("XML:MAX_STEP",3600),
                                 minStep, maxStep, "STEP_RANGE");
    gui1->addWidgetDown(range);
    gui1->addWidgetDown(new ofxUILabel("Individual Position", OFX_UI_FONT_SMALL)); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[0], "0")); 	
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[1], "1")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[2], "2")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[3], "3")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[4], "4")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[5], "5")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[6], "6")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[7], "7")); 
	sliders.push_back(new ofxUISlider(dim,160, 0.0000000f, 1.000000f, currentMotorPosition[8], "8"));
    
    gui1->addWidgetDown(sliders[0]); 	
	gui1->addWidgetRight(sliders[1]); 
	gui1->addWidgetRight(sliders[2]); 
	gui1->addWidgetRight(sliders[3]); 
	gui1->addWidgetRight(sliders[4]); 
	gui1->addWidgetRight(sliders[5]); 
	gui1->addWidgetRight(sliders[6]); 
	gui1->addWidgetRight(sliders[7]); 
	gui1->addWidgetRight(sliders[8]);
    
    matrix = new ofxUIToggleMatrix(dim, dim, 1, 9, "Select Motor");
    gui1->addWidgetDown(matrix);
    //    gui1->addWidgetRight(new ofxUIButton(32, 32, false, "RESET"));
    
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
    ofAddListener(gui1->newGUIEvent, this, &testApp::guiEvent); 
    gui->loadSettings("GUI/guiSettings.xml");
    gui1->loadSettings("GUI/guiSettings1.xml");
    
    
	
	//----------------------------------- note:
	// < this should be set
	// to whatever com port
	// your serial device is
	// connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....   
    
    serial.setup(config.getValue("XML:DEVICE_NAME", "/dev/tty.usbmodemfa141"), 57600);
    
    
    //serial.setup(deviceList[0 ].getDeviceName(), 57600);
    serial.setVerbose(false);
    
    // print input ports to console
	midiIn.listPorts();
	
	// open port by number
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add testApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);
    
    
}
//--------------------------------------------------------------
void testApp::exit()
{
    
    delete gui; 
    delete gui1; 
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    int kind = e.widget->getKind();
    string name = e.widget->getName();
    if(kind == OFX_UI_WIDGET_BUTTON)
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        cout << name << "\t value: " << button->getValue() << endl;
        if(e.widget->getName() == "CLOCKWISE" && button->getValue())
        {
            string message;
            vector<ofxUIToggle *> toggles = matrix->getToggles();
            for(int i = 0 ; i < toggles.size() ; i++)
            {
                
                if(toggles[i]->getValue())
                    message+=ofToString(i)+",D;";
            }
            serial.writeBytes((unsigned char*)message.c_str(),message.length());
            ofLog(OF_LOG_VERBOSE,"serial.writeBytes "+message);
            timeDiff = ofGetElapsedTimef();
            isSent = true;
            
        }
        else if(e.widget->getName() == "COUNTER_CLOCKWISE" && button->getValue())
        {
            string message;
            vector<ofxUIToggle *> toggles = matrix->getToggles();
            for(int i = 0 ; i < toggles.size() ; i++)
            {
                
                if(toggles[i]->getValue())
                    message+=ofToString(i)+",U;";
            }
            serial.writeBytes((unsigned char*)message.c_str(),message.length());
            ofLog(OF_LOG_VERBOSE,"serial.writeBytes "+message);
            timeDiff = ofGetElapsedTimef();
            isSent = true;
        }else if(e.widget->getName() == "CLOCKWISE" || e.widget->getName() == "COUNTER_CLOCKWISE")
        {
            if(!button->getValue())
            {
                string message;
                vector<ofxUIToggle *> toggles = matrix->getToggles();
                for(int i = 0 ; i < toggles.size() ; i++)
                {
                    
                    if(toggles[i]->getValue())
                        message+=ofToString(i)+",S;";
                }
                serial.writeBytes((unsigned char*)message.c_str(),message.length());
                ofLog(OF_LOG_VERBOSE,"serial.writeBytes "+message);
                timeDiff = ofGetElapsedTimef();
                isSent = true;
            }
        }else if(e.widget->getName() == "SEND" && button->getValue())
        {
            ofxUIButton *button = (ofxUIButton*) e.widget;
            if(button->getValue())
            {
                string message;
                vector<ofxUIToggle *> toggles = matrix->getToggles();
                for(int i = 0 ; i < toggles.size() ; i++)
                {
                    int output = ofMap(sliders[i]->getScaledValue(),0.0f,1.0f,range->getScaledValueLow(),range->getScaledValueHigh());
                    //printf("output %i",output);
                    if(toggles[i]->getValue())message+=ofToString(i)+
                        ","+
                        ofToString(output)
                        +";";
                }
                serial.writeBytes((unsigned char*)message.c_str(),message.length());
                ofLog(OF_LOG_VERBOSE,"serial.writeBytes "+message);
                timeDiff = ofGetElapsedTimef();
                isSent = true;
            }
        }else if(e.widget->getName() == "SAVE" && button->getValue())
        {
            gui->saveSettings("GUI/guiSettings.xml"); 
            gui1->saveSettings("GUI/guiSettings1.xml"); 
        }else if(e.widget->getName() == "RESET")
        {
            string s;
            for(int i = 0 ; i < NUM_DEVICE ; i++)
            {
                s += (ofToString(i)+","+"0;");
                
            }
            serial.writeBytes((unsigned char*)s.c_str(),s.length());
        }
    }
    else
    {
        if(e.widget->getName() == "FULLSCREEN")
        {
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            ofSetFullscreen(toggle->getValue());   
        }
        else if(e.widget->getName() == "LOG_LEVEL")
        {
            ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
            vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
            
            for(int i = 0; i < selected.size(); i++)
            {
                ofSetLogLevel((ofLogLevel)i);
            }
        }
        else if(e.widget->getName() == "DEVICE_LIST")
        {
            ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
            vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
            
            for(int i = 0; i < selected.size(); i++)
            {
                cout << "SELECTED: " << selected[i]->getName() << endl; 
                serial.setup(i, 57600);
                break;
            }
        }
        
    }
}
//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    ofLog(OF_LOG_VERBOSE,"Device ID : %i",(int)msg.bytes[2]);
    
    switch(msg.bytes[4])
    {
            //http://web.tiscalinet.it/giordy/midi-tech/showctl.htm
            /*
             01      Lighting          (General Category)
             02      Moving Lights
             03      Colour Changers
             04      Strobes
             05      Lasers
             06      Chasers
             
             10      Sound             (General Category)
             11      Music
             12      CD Players
             13      EPROM Playback
             14      Audio Tape Machines
             15      Intercoms
             16      Amplifiers
             17      Audio Effects Devices
             18      Equalisers
             
             20      Machinery         (General Category)
             21      Rigging
             22      Flys
             23      Lifts
             24      Turntables
             25      Trusses
             26      Robots
             27      Animation
             28      Floats
             29      Breakaways
             2A      Barges
             
             30      Video              (General Category)
             31      Video Tape Machines
             32      Video Cassette Machines
             33      Video Disc Players
             34      Video Switchers
             35      Video Effects
             36      Video Character Generators
             37      Video Still Stores
             38      Video Monitors
             
             40      Projection         (General Category)
             41      Film Projectors
             42      Slide Projectors
             43      Video Projectors
             44      Dissolvers
             45      Shutter Controls
             
             50      Process Control     (General Category)
             51      Hydraulic Oil
             52      H20
             53      CO2
             54      Compressed Air
             55      Natural Gas
             56      Fog
             57      Smoke
             58      Cracked Haze
             
             60      Pyro                 (General Category)
             61      Fireworks
             62      Explosions
             63      Flame
             64      Smoke pots
             
             7F      All-types
             */
        case 0x02:
            ofLog(OF_LOG_VERBOSE,"Lighting Cue (General)");
            break;
        case 0x03:
            ofLog(OF_LOG_VERBOSE,"Lighting Cue Color Change");
            break;
        case 0x04:
            ofLog(OF_LOG_VERBOSE,"Lighting Cue Color Change");
            break;
        case 0x05:
            ofLog(OF_LOG_VERBOSE,"Lighting Cue Color Change");
            break;
        case 0x06:
            ofLog(OF_LOG_VERBOSE,"Lighting Cue Color Change");
            break;
        case 0x07:
            ofLog(OF_LOG_VERBOSE,"Lighting Cue Chaser");
            break;
        case 0x20:
            ofLog(OF_LOG_VERBOSE,"Machinery(General)");
            break;
        case 0x26:
            ofLog(OF_LOG_VERBOSE,"Robot Cue");
            break;
    }
    switch(msg.bytes[5])
    {
            //http://figure53.com/qlab/documentation/msccue.php
            /*GO
             STOP
             RESUME
             TIMED_GO
             LOAD
             SET
             FIRE
             ALL_OFF
             RESTORE
             RESET
             GO_OFF
             GO/JAM_CLOCK
             STANDBY_+
             STANDBY_-
             SEQUENCE_+
             SEQUENCE_-
             START_CLOCK
             STOP_CLOCK
             ZERO_CLOCK
             SET_CLOCK
             MTC_CHASE_ON
             MTC_CHASE_OFF
             OPEN_CUE_LIST
             CLOSE_CUE_LIST
             OPEN_CUE_PATH
             CLOSE_CUE_PATH
             */
            
        case 0x01:
            ofLog(OF_LOG_VERBOSE,"GO");
            timeline.play();
            break;
        case 0x02:
            ofLog(OF_LOG_VERBOSE,"STOP");
            timeline.stop();
            break;
        case 0x03:
            ofLog(OF_LOG_VERBOSE,"RESUME");
            break;
        case 0x04:
            ofLog(OF_LOG_VERBOSE,"TIMED_GO");
            break;
        case 0x05:
            ofLog(OF_LOG_VERBOSE,"LOAD");
            break;
        case 0x06:
            ofLog(OF_LOG_VERBOSE,"SET");
            break;
        case 0x07:
            ofLog(OF_LOG_VERBOSE,"FIRE");
            break;
        case 0x08:
            ofLog(OF_LOG_VERBOSE,"ALL_OFF");
            break;
        case 0x09:
            ofLog(OF_LOG_VERBOSE,"RESTORE");
            break;
        case 0x10:
            ofLog(OF_LOG_VERBOSE,"RESET");
            break;
        case 0x11:
            ofLog(OF_LOG_VERBOSE,"GO_OFF");
            break;
        case 0x12:
            ofLog(OF_LOG_VERBOSE,"GO/JAM_CLOCK");
            break;
        default:
            ofLog(OF_LOG_VERBOSE,"UNKNOW COMMAND");
            break;
    }
    
}
//--------------------------------------------------------------
void testApp::update(){
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
    if(strlen(bytesReadString)>0)
    {
        
        ofLog(OF_LOG_VERBOSE,"%s",bytesReadString);
        
        if(isSent)
        {
            float  diff = ofGetElapsedTimef()-timeDiff;
            ofLog(OF_LOG_VERBOSE,"time difference :%f",diff);
            isSent = false;
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    timeline.draw();
}

//--------------------------------------------------------------
void testApp::receivedTrigger(ofxTLTriggerEventArgs& trigger){
    ofLog(OF_LOG_VERBOSE,"Trigger from " + trigger.triggerGroupName +" triggerName " + trigger.triggerName );
    //    const char *delim = ",;";
    //    char * pch;
    //    char str[trigger.triggerName.length()];
    //    memcpy(str, trigger.triggerName.c_str(), trigger.triggerName.length());
    //    printf ("Splitting string \"%s\" into tokens:\n",str);
    //    pch = strtok(str,delim);
    
    //    while (pch != NULL)
    //    {
    //        printf ("%s\n",pch);
    //        pch = strtok (NULL, delim);
    //    }      
    
    if(trigger.triggerName=="STOP")
    {
        timeline.stop();
    }
    else
    {
        string s (trigger.triggerName+";");
        
        timeDiff = ofGetElapsedTimef();
        serial.writeBytes((unsigned char*)s.c_str(),s.length());
        ofLog(OF_LOG_VERBOSE,"serial.writeBytes "+s);
        isSent = true;
    }
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 27:
        {
            gui->saveSettings("GUI/guiSettings.xml"); 
            gui1->saveSettings("GUI/guiSettings1.xml"); 
            string s;
            for(int i = 0 ; i < NUM_DEVICE ; i++)
            {
                s += (ofToString(i)+","+"0;");
                
            }
            serial.writeBytes((unsigned char*)s.c_str(),s.length());
            ofLog(OF_LOG_VERBOSE,"serial.writeBytes "+s);
            //ofSleepMillis(5000);
        }
            break;
            
        case ' ':
            timeline.togglePlay();
            break;
        case 'h':
            timeline.toggleShow();
            break;
        case '\t':
            gui->toggleVisible(); 
            gui1->toggleVisible(); 
            
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