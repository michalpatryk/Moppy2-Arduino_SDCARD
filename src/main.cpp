#include <Arduino.h>

#include <avr/pgmspace.h>
#include "MoppyConfig.h"
#include "MoppyInstruments/MoppyInstrument.h"
#include "CustomSongTables.h"
#include "MoppySD/SDMacros.h"
/**********
 * MoppyInstruments handle the sound-creation logic for your setup.  The
 * instrument class provides a systemMessage handler function and a deviceMessage
 * handler function for handling messages received by the network and a tick
 * function for precise timing events.
 *
 * Configure the appropriate instrument class for your setup in MoppyConfig.h
 */

// Floppy drives directly connected to the Arduino's digital pins
#ifdef INSTRUMENT_FLOPPIES
#include "MoppyInstruments/FloppyDrives.h"
MoppyInstrument *instrument = new instruments::FloppyDrives();
#endif

// EasyDriver stepper motor driver
#ifdef INSTRUMENT_EASYDRIVER
#include "MoppyInstruments/EasyDrivers.h"
MoppyInstrument *instrument = new instruments::EasyDrivers();
#endif

// L298N stepper motor driver
#ifdef INSTRUMENT_L298N
#include "MoppyInstruments/L298N.h"
MoppyInstrument *instrument = new instruments::L298N();
#endif

// _A single device (_e.g. xylophone, drums, etc.) connected to shift registers
#ifdef INSTRUMENT_SHIFT_REGISTER
#include "MoppyInstruments/ShiftRegister.h"
MoppyInstrument *instrument = new instruments::ShiftRegister();
#endif

// Floppy drives connected to 74HC595 shift registers
#ifdef INSTRUMENT_SHIFTEdFLOPPIES
#include "MoppyInstruments/ShiftedFloppyDrives.h"
MoppyInstrument *instrument = new instruments::ShiftedFloppyDrives();
#endif

/**********
 * MoppyNetwork classes receive messages sent by the Controller application,
 * parse them, and use the data to call the appropriate handler as implemented
 * in the instrument class defined above.
 *
 * Configure the appropriate networking class for your setup in MoppyConfig.h
 */

// Standard Arduino HardwareSerial implementation
#ifdef NETWORK_SERIAL
#include "MoppyNetworks/MoppySerial.h"
//MoppySerial network = MoppySerial(instrument);
#endif

//// UDP Implementation using some sort of network stack?  (Not implemented yet)
#ifdef NETWORK_UDP
#include "MoppyNetworks/MoppyUDP.h"
MoppyUDP network = MoppyUDP(instrument);
#endif

#ifdef SDCARD
#include "MoppySD/MoppySD.h"
MoppySD sdManager = MoppySD(instrument);
#endif

void blinkLEDMain() {
    digitalWrite(13, HIGH); // set the LED on
    delay(250);              // wait for a second
    digitalWrite(13, LOW);
    delay(250);
}
//void printDirectory(File dir, int numTabs) {
//    Serial.println("123");
//    while (true) {
//        File entry = dir.openNextFile();
//        if (!entry) {
//            // no more files
//            Serial.println("No more files");
//            break;
//        }
//        for (uint8_t i = 0; i < numTabs; i++) {
//            Serial.print('\t');
//        }
//        Serial.print(entry.name());
//        if (entry.isDirectory()) {
//            Serial.println("/");
//            printDirectory(entry, numTabs + 1);
//        }
//        else {
//            // files have sizes, directories do not
//            Serial.print("\t\t");
//            Serial.println(entry.size(), DEC);
//        }
//        entry.close();
//    }
//}

//The setup function is called once at startup of the sketch
void setup()
{
    // Call setup() on the instrument to allow to to prepare for action
    instrument->setup();
	
    pinMode(BTN_R, INPUT);
    pinMode(BTN_M, INPUT);
    pinMode(BTN_L, INPUT);
    pinMode(ATX, OUTPUT);
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
	if(DEBUG_SERIAL)
	{
        Serial.begin(9600);
        while (!Serial) {
            ; // wait for serial port to connect. Needed for native USB port only
        }
	}
    sdManager.enableATX();
    sdManager.startupSD();
    //sdManager.debugPlay();
 //   //myFile = SD.open("/");
 //   //printDirectory(myFile, 0);
    
    
}


// The loop function is called in an endless loop
void loop()
{
    sdManager.loopEvent();
	//Check for any buttons pressed
	//if not -> song management
	//if yes -> reset all flopps -> button management
	//
	//  song management:
	//  load sd card and place data into memory - 2x1024B(note) 2x2048B(duration)?
	//      start playing notes. When on specific point of memory buffer (half?) read again

	// Endlessly read messages on the network.  The network implementation
	// will call the system or device handlers on the intrument whenever a message is received.
    //network.readMessages();

}
