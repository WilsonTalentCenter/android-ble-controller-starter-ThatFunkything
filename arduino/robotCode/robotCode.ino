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

const int frontLeftIn1 = 36;
const int frontLeftIn2 = 39;
const int frontLeftSpeed = 6; //enable

const int frontRightIn1 = 34;
const int frontRightIn2 = 38;
const int frontRightSpeed = 12; //enable

const int backLeftIn1 = 37;
const int backLeftIn2 = 41;
const int backLeftSpeed = 7; //enable

const int backRightIn1 = 36;
const int backRightIn2 = 40;
const int backRightSpeed = 13; //enable

const int er = 53;



void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

    if(inputString=="FLD"){
      digitalWrite(frontLeftIn1,HIGH);
      digitalWrite(frontLeftIn2,LOW);
      analogWrite(frontLeftSpeed,255);

      digitalWrite(frontRightIn1,HIGH);
      digitalWrite(frontRightIn2,LOW);
      analogWrite(frontRightSpeed,255);

      digitalWrite(backLeftIn1,HIGH);
      digitalWrite(backLeftIn2,LOW);
      analogWrite(backLeftSpeed,255);

      digitalWrite(backRightIn1,HIGH);
      digitalWrite(backRightIn2,LOW);
      analogWrite(backRightSpeed,255);
    }
    else if(inputString=="FLU"){
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
    }

    if(inputString.startsWith("LJ")){
      //for left joystick


      int x;
      int y;

    }
    // clear the string:
    inputString = "";
    stringComplete = false;
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
