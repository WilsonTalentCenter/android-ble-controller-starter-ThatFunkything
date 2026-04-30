/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/communication/SerialEvent/
*/

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

const int frontLeft[3] = {35, 39, 6};
const int frontLeftIn1 = 35;
const int frontLeftIn2 = 39;
const int frontLeftSpeed = 6; //enable

//backward
const int frontRight[3] = {34, 38, 12};
const int frontRightIn1 = 34;
const int frontRightIn2 = 38;
const int frontRightSpeed = 12; //enable

const int backLeft[3] = {37, 41, 7};
const int backLeftIn1 = 37;
const int backLeftIn2 = 41;
const int backLeftSpeed = 7; //enable

//backward
const int backRight[3] = {36, 40, 13};
const int backRightIn1 = 36;
const int backRightIn2 = 40;
const int backRightSpeed = 13; //enable

const int er = 53;



void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(er,OUTPUT);
  pinMode(frontRightIn1, OUTPUT);
  pinMode(frontRightIn2, OUTPUT);
  pinMode(backRightIn1, OUTPUT);
  pinMode(backRightIn2, OUTPUT);
  pinMode(frontLeftIn1, OUTPUT);
  pinMode(frontLeftIn2, OUTPUT);
  pinMode(backLeftIn1, OUTPUT);
  pinMode(backLeftIn2, OUTPUT);

  digitalWrite(er,HIGH);
  digitalWrite(frontLeftIn1,LOW);
  digitalWrite(frontLeftIn2,LOW);
  analogWrite(frontLeftSpeed,0);

  digitalWrite(frontRightIn1,LOW);
  digitalWrite(frontRightIn2,LOW);
  analogWrite(frontRightSpeed,0);

  digitalWrite(backLeftIn1,LOW);
  digitalWrite(backLeftIn2,LOW);
  analogWrite(backLeftSpeed,0);

  digitalWrite(backRightIn1,LOW);
  digitalWrite(backRightIn2,LOW);
  analogWrite(backRightSpeed,0);



  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

//buttons
//forward
    if(inputString=="FLD\n"){
      moveWheel(frontLeft, 255, true, true);
      moveWheel(backLeft, 255, true, true);
      moveWheel(frontRight, 255, true, false);
      moveWheel(backRight, 255, true, false);

    }
    else if(inputString=="FLU\n"){
      moveWheel(frontLeft, 0, true, true);
      moveWheel(backLeft, 0, true, true);
      moveWheel(frontRight, 0, true, false);
      moveWheel(backRight, 0, true, false);

    }

//backward
    if(inputString=="1\n"){
      moveWheel(frontLeft, 255, true, false);
      moveWheel(backLeft, 255, true, false);
      moveWheel(frontRight, 255, true, true);
      moveWheel(backRight, 255, true, true);

    }
    else if(inputString=="0\n"){
      moveWheel(frontLeft, 0, true, true);
      moveWheel(backLeft, 0, true, true);
      moveWheel(frontRight, 0, true, false);
      moveWheel(backRight, 0, true, false);

    }

//joystick
    if(inputString.startsWith("LJ")){
      //for left joystick
      //300 is a placeholder to check if they have been given a new number
      int x = 300;
      int y = 300;

      String del = ",";
      int pos = inputString.indexOf(",");

      inputString.remove(0, pos);
      
      int stringStart = 0;
      for(int i = 0; i < inputString.length(); i++){
        if (inputString.charAt(i) == ","){
          int end = i;

          if (x == 300){
            x = atoi(inputString.substring(stringStart, end).c_str());
            stringStart = i + 1;
            
          }
          else if (y == 300){
            y = atoi(inputString.substring(stringStart, end).c_str());
          }
        }
      }
      

      //UNFINISHED
      /* make the robot move sideways depending on what direction the joystick is moved & make the wheels stop */
      x *= 2;
      y *= 2;
      boolean forwards = true;
      boolean right = true;
      if (x < 0){
        x *= -1;
        right = false;
      }
      if (y < 0){
        x *= -1;
        forwards = false;
      }
      moveWheel(frontLeft, y, forwards, false);
      moveWheel(backLeft, y, forwards, false);
      moveWheel(frontRight, y, forwards, true);
      moveWheel(backRight, y, forwards, true);

    }
    else{
      //fix this, it probably doesn't stop the wheels correctly
      moveWheel(frontLeft, 0, true, false);
      moveWheel(backLeft, 0, true, false);
      moveWheel(frontRight, 0, true, true);
      moveWheel(backRight, 0, true, true);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


//assumes speed is 0-255
void moveWheel(int motor[], int speed, bool forward, bool isBackwards){

  if(speed == 0){
    digitalWrite(motor[0],LOW);
    digitalWrite(motor[1],LOW);
    analogWrite(motor[2],speed);
  }
  else{
  if(isBackwards){
    digitalWrite(motor[0],(forward ? LOW : HIGH));
    digitalWrite(motor[1],(forward ? HIGH : LOW));
    analogWrite(motor[2],speed);
  }
  else{
    digitalWrite(motor[0],(forward ? HIGH : LOW));
    digitalWrite(motor[1],(forward ? LOW : HIGH));
    analogWrite(motor[2],speed);
  }
  }
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
