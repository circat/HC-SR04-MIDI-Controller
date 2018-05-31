#include <TM1637Display.h>
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
#define SONAR_NUM 1      // Number of sensors.
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

int SEN1 = 0;

medianFilter Filter;

int filtered;

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(13, 12, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
};

void setup() {

  Serial.begin(31250);
   //Serial.begin(115200);
  Filter.begin();

}

void loop() { 
  
display.setBrightness(0x0f);
SEN1 = sonar[0].ping_median(3);


SEN1 = sonar[0].convert_cm(SEN1); // newping CONVERT TO CM
filtered= Filter.run(SEN1);
int CC20 = map(constrain(filtered, 0, 50), 0, 49, 127, 1);


if((filtered  > 1) && (filtered < 127)){
  
sendMIDI(CC, 1, 20, CC20); // CH1 , CCNr20,value
display.showNumberDec(CC20,false);
}  
}
