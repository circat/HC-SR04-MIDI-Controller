#include <TM1637Display.h>
#include <SoftwareSerial.h>
#include <NewPing.h>
#include <medianFilter.h>
#include <sendMIDI.h>



/*#########################
 * sendMIDI.h =
 * 
 * sendMIDI(CC, Channel, CC Number, Value 0-127);
 * const uint8_t NOTE_OFF = 0x80;
 * const uint8_t NOTE_ON = 0x90;
 * const uint8_t KEY_PRESSURE = 0xA0;
 * const uint8_t CC = 0xB0;
 * const uint8_t PROGRAM_CHANGE = 0xC0;
 * const uint8_t CHANNEL_PRESSURE = 0xD0;
 * const uint8_t PITCH_BEND = 0xE0;
 */
#define SONAR_NUM 2      // Number of sensors.
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

int SEN1 = 0;
int SEN2 = 0;

medianFilter Filter;
medianFilter Filter2;

int filtered;

int filtered2;
//const int PIEZO_PIN = A0; // Piezo output

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(13, 12, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(9, 10, MAX_DISTANCE), 
  ///NewPing(9, 10, MAX_DISTANCE)
};

void setup() {

  Serial.begin(31250);
   //Serial.begin(115200);
  Filter.begin();
  Filter2.begin();
}

void loop() { 
display.setBrightness(0x0f);

  SEN1 = sonar[0].ping_median(3);

  SEN2 = sonar[1].ping_median(3);

 // int piezoADC = analogRead(PIEZO_PIN);
 // float piezoV = piezoADC / 1023.0 * 5.0;
  //Serial.println(piezoV); // Print the voltage.
  
SEN1 = sonar[0].convert_cm(SEN1); // newping CONVERT TO CM
SEN2 = sonar[1].convert_cm(SEN2); // newping CONVERT TO CM

filtered= Filter.run(SEN1);
filtered2= Filter2.run(SEN2);

int CC20 = map(constrain(filtered, 0, 50), 0, 49, 127, 1);
int CC21 = map(constrain(filtered2, 0, 50), 0, 49, 127, 1);

if((filtered  > 1) && (filtered < 127)){

sendMIDI(CC, 1, 20, CC20); // CH1 , CCNr20,value

//sendMIDI(NOTE_ON, 1, CC20, 100); //

display.showNumberDec(CC20,false);
//Serial.println(CC20);
// Serial.print("\t");
//Serial.println(SEN2);
}  

/*if((filtered2  > 1) && (filtered2 < 127)){
sendMIDI(CC, 1, 1, CC21);
//Serial.println(CC21);
}*/

/*if((filtered  == 0)) {
  
//sendMIDI(NOTE_OFF, CC20,0);
//display.showNumberDec(0,true);
//Serial.println(CC21);
}*/

}
