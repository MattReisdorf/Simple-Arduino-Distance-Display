
#include <Wire.h> //include Wire library
#define disp 0x72 //I2C scan to check display address

const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int LED_ALARM = 4;
const unsigned int MAX_DIST = 23200; //Echo signal formula uS/58 = cm
                                     //4m = 400cm => 23200uS Echo
char distance [10]; 


void setup() {
  pinMode(TRIG_PIN, OUTPUT);     //Output Trigger signal
  pinMode(LED_ALARM, OUTPUT);    //Output LED Alarm signal
  pinMode(ECHO_PIN, INPUT);      //Input Echo signal
  digitalWrite(TRIG_PIN, LOW);   //Initialize Trigger low
  Serial.begin(9600);
  clearDisplay();                //Function call to clear 7SD
  setBrightness(255);            //Function call to set 7SD brightness
}

void loop() {
  unsigned long time1;           //32 bit integer to hold low Echo time
  unsigned long time2;           //32 bit integer to hold high Echo time
  unsigned long pulseWidth;      //32 bit integer holds pulse width between high and low times
  float cm;
  
  digitalWrite(TRIG_PIN, HIGH);  
  delayMicroseconds(10);         //10uS LOW->HIGH->LOW pulse; activates the distance sensor
  digitalWrite(TRIG_PIN, LOW);
  
  while (digitalRead(ECHO_PIN) == 0) {  //tracks low time before echo receives returning signal
    time1 = micros();
  }
  
  
  while (digitalRead(ECHO_PIN) == 1) {  //tracks high time on echo pin
    time2 = micros();
  }
  
  
  pulseWidth = time1 - time2;     //difference between high and low times is the pulsewidth of the signal
  cm = pulseWidth / 58.0;         //conversion to cm from formula in line 
  
  Serial.println(cm);             //print distance to serial monitor
  
  if (cm < 4) {
    digitalWrite(LED_ALARM, HIGH);   //Alarm if too close
  }

  if (cm > 400) {
    digitalWrite(LED_ALARM, HIGH);   //Alarm if too far
  }
  
  sprintf(distance, "%4d", cm);      
  
  sendToDisplay(distance);        //Sends the distance in cm to the 7SD
  delay(500);                     //500 uS delay in loop to prevent echo signals from overlapping
}



//Functions

void sendToDisplay(String toSend) {  //function sends message to the 7SD
  Wire.beginTransmission(disp);
  for (int i=0;i<3;i++) {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

void clearDisplay() {                //function clears 7SD
  Wire.beginTransmission(disp);
  Wire.write(0x76);                  //bytecode 0x76 actually clears
  Wire.endTransmission();
}

void setBrightness(byte setting) {   //function sets the brightness
  Wire.beginTransmission(disp);
  Wire.write(0x7A);                  //bytecode 0x7A sets to max
  Wire.write(setting);
  Wire.endTransmission();
}
