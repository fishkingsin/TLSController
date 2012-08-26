//http://www.open.com.au/mikem/arduino/AccelStepper/
#include <AccelStepper.h>
#define NUM_DEVICE 9
// Define two steppers and the pins they will use
AccelStepper stepper1(1, 3 ,2);
AccelStepper stepper2(1, 5, 4);
AccelStepper stepper3(1, 7, 6);
AccelStepper stepper4(1, 9, 8);
AccelStepper stepper5(1, 11, 10);
AccelStepper stepper6(1, 13, 12);
AccelStepper stepper7(1, 15, 14);
AccelStepper stepper8(1, 17, 16);
AccelStepper stepper9(1, 19, 18);
AccelStepper steppers[NUM_DEVICE] = {
  stepper1,
  stepper2,stepper3,stepper4,
  stepper5,stepper6,stepper7,stepper8,stepper9
};
long maxSspeed[9];
long cSpeed[9];
String inString = "";    // string to hold input
int currentColor = 0;
int id = 0;
boolean isConstant[9];
long curSpeed[9];
long curAccel[9];
long curPos[9];

void setup() {

  Serial.begin(57600);
  for(int i  = 0 ; i < NUM_DEVICE ; i++)
  {
    isConstant[i] = false;
    steppers[i].setMaxSpeed(10000);
    steppers[i].setAcceleration(2000);
    curSpeed[i] = 10000;
    curAccel[i] = 2000;
    curPos[i] = 0;

  }
}

void loop() {
  int inChar;

  // Read serial input:
  if (Serial.available() > 0) {
    inChar = Serial.read();

  }

  if (isDigit(inChar) || inChar==0x2d || inChar=='D' || inChar=='S' || inChar=='U') {
    inString += (char)inChar;
  }
  if (inChar == ',') {
    switch (currentColor) {
    case 0:
      id = inString.toInt();
      inString = ""; 
      break;
    case 1:
      curSpeed[id] = satoi(inString)*100;
      steppers[id].setMaxSpeed(curSpeed[id]);
      //Serial.print("Max Speed: ");
      //Serial.println(satoi(inString));
      inString = "";           
      break;
    case 2:
      curAccel[id] = satoi(inString)*100;
      steppers[id].setAcceleration(curAccel[id]);
      //Serial.print("Acceleration: ");
      //Serial.println(satoi(inString));
      inString = ""; 
      break;
    }
    currentColor++;
  }
  if (inChar == ';' ) {
    switch(inString[0])
    {
    case 'S':
      isConstant[id] = false;
      steppers[id].move(0);
//      steppers[id].setSpeed(0);
//      steppers[id].setAcceleration(0);
//      steppers[id].setMaxSpeed(0);
      Serial.print(id);
      Serial.println(" : Stop Motor");
      Serial.print("CurrentSpeed : ");
      Serial.println(steppers[id].speed());
      
      break;
    case 'U':
      isConstant[id] = true;
      steppers[id].setSpeed(curSpeed[id]);
      Serial.print(id);
      Serial.println(" : CLOCKWISE MOTOR");
      break;
    case 'D':
      isConstant[id] = true;
      steppers[id].setSpeed(-curSpeed[id]);
      Serial.print(id);
      Serial.println(" : COUNTERCLOCKWISE MOTOR");
      break;
    default:

      long _step_ = satoi(inString);
      Serial.print("id: ");
      Serial.println(id);
      Serial.print("step: ");
      Serial.println(_step_);

      curPos[id] = _step_;
      steppers[id].moveTo(_step_);
      break;
    }
    inString = ""; 
    currentColor = 0;
  }
  for(int i  = 0 ; i < NUM_DEVICE ; i++)
  {
    if(isConstant[i])
    {
      steppers[i].runSpeed();
    }
    else
    {
      steppers[i].run();
    }
  }

}

long satoi( String str ){  
  long r = 0; 
  long netgative =  1;
  int len = str.length();  
  for(int i=0; i<len; i++){  

    if ( str[i] < 0x3a && str[i] > 0x2f){  
      // is a ASCII number, return it  
      r = r * 10;  
      r += (str[i]-0x30);  
    }
    else if (str[i] == 0x2d){
      netgative = -1;
    }
    else{  
      i = len; //exit!  
      r = -1;  
      break;  
    }    
  }  
  return r*netgative;   
}  





