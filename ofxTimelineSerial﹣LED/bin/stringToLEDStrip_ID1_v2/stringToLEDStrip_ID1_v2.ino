// color swirl! connect an RGB LED to the PWM pins as indicated
// in the #defines
// public domain, enjoy!

#define REDPIN 5
#define GREENPIN 3
#define BLUEPIN 6
String inString = "";    // string to hold input

int count = 0;
int red, green, blue = 0;
boolean isStart;
int myid = 1;
int id = -1;
void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(57600);
}


void loop() {
  int inChar;

  // Read serial input:
  if (Serial.available() > 0) {
    inChar = Serial.read();
  }
  if(inChar ==254)
  {
    isStart = true;
    count = 0;
  }
  else if(inChar==255)
  {
    isStart = false;
  }
  if(isStart)
  {
    switch(count)
    {
    case 0:
      id = inChar;
      break;
    case 1:
      red = inChar*2;
      break;
    case 2:
      green = inChar*2;
      break;
    case 3:
      blue = inChar*2;
      break;
    }
    if(id==myid && count==3)
    {
      // set the levels of the LED.
      // subtract value from 255 because a higher
      // analogWrite level means a dimmer LED, since
      // you're raising the level on the anode:
      analogWrite(REDPIN, red);
      analogWrite(BLUEPIN, blue);
      analogWrite(GREENPIN, green);

      // print the colors:
      Serial.print("id: ");
      Serial.print(id  );
      Serial.print("Red: ");
      Serial.print(red);
      Serial.print(", Green: ");
      Serial.print(green);
      Serial.print(", Blue: ");
      Serial.println(blue);
    }
    count++;
    count%=4;
  }

  //  if (isDigit(inChar)) {
  //    // convert the incoming byte to a char 
  //    // and add it to the string:
  //    inString += (char)inChar; 
  //  }
  //
  //  // if you get a comma, convert to a number,
  //  // set the appropriate color, and increment
  //  // the color counter:
  //  if (inChar == ',') {
  //    // do something different for each value of currentColor:
  //    switch (currentColor) {
  //    case 0:    // 0 = red
  //      id = inString.toInt();
  //      // clear the string for new input:
  //      inString = ""; 
  //      break;
  //    case 1:    // 0 = red
  //      red = inString.toInt();
  //      // clear the string for new input:
  //      inString = ""; 
  //      break;
  //    case 2:    // 1 = green:
  //      green = inString.toInt();
  //      // clear the string for new input:
  //      inString = ""; 
  //      break;
  //    }
  //    currentColor++;
  //  }
  //  // if you get a newline, you know you've got
  //  // the last color, i.e. blue:
  //  if (inChar == '\n') {
  //    blue = inString.toInt();

  //    // clear the string for new input:
  //    inString = ""; 
  //    // reset the color counter:
  //    currentColor = 0;
  //
  //  }
}











