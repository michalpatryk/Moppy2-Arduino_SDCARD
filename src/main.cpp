#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
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
struct FMHeader
{
    uint8_t sizeHeader;
    uint8_t blockWidth;
    uint8_t baseDuration;
    uint16_t notesCount;
};
struct FMBlock
{
    uint8_t duration1[64];
    uint8_t note1[64];
    uint8_t duration2[64];
    uint8_t note2[64];
};

File myFile;
FMHeader fmHeader;
FMBlock fmBlock;

void setup()
{
    pinMode(BTN_R, INPUT);
    pinMode(BTN_M, INPUT);
    pinMode(BTN_L, INPUT);
    pinMode(ATX, OUTPUT);
    pinMode(SS, OUTPUT);
    //digitalWrite(ATX, HIGH);
	
    Serial.begin(9600);
    
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    sdManager.enableATX();
    Serial.print(F("Initializing SD card..."));
    if (!SD.begin(SS)) {
        Serial.println(F("initialization failed!"));
        while (1);
    }

    Serial.println(F("initialization done."));
 //   //myFile = SD.open("/");
 //   //printDirectory(myFile, 0);
    myFile = SD.open(F("/64BTET.fm"));
 //   
 //   
	myFile.read(&fmHeader, 5);
    Serial.println(fmHeader.sizeHeader);
    Serial.println(fmHeader.blockWidth);
    Serial.println(fmHeader.baseDuration);
    Serial.println(fmHeader.notesCount);
    myFile.read(&fmBlock, 256);
    Serial.println(F("Duration1: "));
    Serial.println(fmBlock.duration1[0]);
    Serial.println(fmBlock.duration1[1]);
    Serial.println(fmBlock.duration1[2]);
    Serial.println(fmBlock.duration1[3]);
    // Call setup() on the instrument to allow to to prepare for action
    instrument->setup();

    // setup button pins
    // btn R - pc0 (14)
    // btn M - pc1 (15)
    // brn L - pc2 (16)
    // setup atx pin to high
	
    //_E B _C B _C B _A _A _C _E _D _C B _C _D _E _C _A _A _D _F a _G _F _E _C _E _D _C B _C _D _E _C _A _A
    //_E........._A............._E......._A....._D........._C........._E......._A...

 //   uint8_t nullLoad[] = { 0 };
 //   unsigned long lastRun = 0;
	////bring the second floppy down an octave

    //for (int i = 0; i < fmHeader.notesCount; i++)
    for (int i = 0; i < 5; i++)
    {
        //Serial.println(F("Note:"));
        //Serial.println(fmBlock.note1[i]);
        //Serial.println(F("BaseDuration:"));
        //Serial.println((fmHeader.baseDuration));
        //Serial.println(F("Mult:"));
        //Serial.println(fmBlock.duration1[i]);
        //Serial.println(F("Delay:"));
        //Serial.println(fmHeader.baseDuration * fmBlock.duration1[i]);
        //instrument->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, tetrisNoteFloppy1 + i);
        instrument->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note1 + i);
        //instrument->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note2 + i);
        //delay(tetrisDurationFloppy1[i]*4);
        delay(fmHeader.baseDuration * fmBlock.duration1[i] * 2);
        instrument->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
        //blinkLEDMain();
        delay(50);
    }

}


// The loop function is called in an endless loop
void loop()
{
    sdManager.loopEvent();
    
    //Serial.println(F("Loop end"));
    //
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
