#include <Arduino.h>

#include "MoppySD.h"
#include "SDMacros.h"
MoppySD::MoppySD(MoppyMessageConsumer* messageConsumer)
{
	targetConsumer = messageConsumer;
}

void MoppySD::enableATX()
{
    if (DEBUG_SERIAL)
    {
        Serial.println(F("Enabling ATX"));
    }
	digitalWrite(ATX, HIGH);
	
}

void MoppySD::disableATX()
{
    if (DEBUG_SERIAL)
    {
        Serial.println(F("Disabling ATX"));
    }
    digitalWrite(ATX, LOW);
}
void printDirectory(File dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            if (numTabs == 0)
                Serial.println("** Done **");
            return;
        }
        for (uint8_t i = 0; i < numTabs; i++)
            Serial.print('\t');
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("isDir");
            //printDirectory(entry, numTabs + 1);
        }
        else
        {
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void MoppySD::startupSD()
{
    if (DEBUG_SERIAL)
    {
        Serial.print(F("Initializing SD card..."));
    }
    if (!SD.begin(SS)) {
        if (DEBUG_SERIAL)
        {
            Serial.println(F("initialization failed!"));
        }     
        while (1)
        {
            uint8_t note[] = { 40 };
            targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, note);
            delay(200);
            targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
            delay(1000);
        };
    }
    
    if (DEBUG_SERIAL)
    {
        Serial.println(F("initialization done."));
    }
    //myFile = SD.open("/");
    //myFile = myFile.openNextFile();
}
void MoppySD::startSong()
{
    if (DEBUG_SERIAL)
    {
        Serial.println(filePos);
    }
    isNowPlaying = true;
	if(filePos == 0)
	{
        myFile = SD.open(F("/64BTET.fm"));
	}
    else if(filePos == 1)
    {
	    myFile = SD.open(F("/64BPIR.fm"));
    }
    else if(filePos == 2)
    {
        //uint8_t note[] = { 44 };
        //targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, note);
        //delay(200);
        //targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
        //delay(1000);
        //myFile = SD.open(F("/64BPIR.fm"));
	    myFile = SD.open(F("/64BSTL.fm"));
    }
    else
    {
        char name[6] = { '0', '0', '0', '.', 'f','m' };
        name[2] += filePos - 2;
        myFile = SD.open(name);
    }
    //myFile = SD.open("/");
    //printDirectory(myFile, 0);
    //myFile.close();
//    Serial.println("My:");
 //   myFile = SD.open("/");
	//for(int i = 0; i<3; i++)
	//{
 //       Serial.println("Start of loop: ");
 //       Serial.println(myFile.name());
 //       File entry = myFile.openNextFile();
 //       Serial.println(myFile.name());
 //       Serial.println(entry.name());
	//}
 //   

    //myFile = SD.open(F("/64BTET.fm")); //TODO its for debug
    //myFile = SD.open(F("/64BPIR.fm"));
    myFile.read(&fmHeader, 5);
    if (DEBUG_SERIAL)
    {
        Serial.println(fmHeader.sizeHeader);
        Serial.println(fmHeader.blockWidth);
        Serial.println(fmHeader.baseDuration);
        Serial.println(fmHeader.notesCount);
    }

    myFile.read(&fmBlock, 256);
    if (DEBUG_SERIAL)
    {
        Serial.println(F("Duration1: "));
        Serial.println(fmBlock.duration1[0]);
        Serial.println(fmBlock.duration1[1]);
        Serial.println(fmBlock.duration1[2]);
        Serial.println(fmBlock.duration1[3]);
    }
    messagePos1 = 0;    // position on note1/duration1
    messagePos2 = 0;    // position on note2/duration2
    floppy1Time = millis();
    floppy2Time = floppy1Time;
    targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note1 + messagePos1);
    targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note2 + messagePos2);
    myFile.close();
    //floppy1Time = 0;
    //floppy2Time = 0;
}
void MoppySD::loopEvent()
{
    int btnL = digitalRead(BTN_L);
    int btnM = digitalRead(BTN_M);
    int btnR = digitalRead(BTN_R);
    uint8_t payload[] = { _C , _D, _E };
    if (btnL == LOW)
    {
        if (DEBUG_SERIAL)
        {
            Serial.println(F("BtnL working"));
            Serial.println(filePos);
        }
    	//PREVIOUS SONG
        //targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, payload);
        if(filePos == 0)
        {
            filePos = tracksNum;
        }
        else
        {
            filePos -= 1;
        }
        delay(200);
        if (DEBUG_SERIAL)
        {
            Serial.println(F("After"));
            Serial.println(filePos);
        }
    }
    else if (btnM == LOW)
    {
        if (DEBUG_SERIAL)
        {
        	
            Serial.println(F("BtnM working"));
        }
    	//START/STOP
    	if(isNowPlaying)    //is playing so we stop
    	{
            isNowPlaying = false;
            targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
            targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);        
            disableATX();
            delay(200);
    	}
        else
        {
            enableATX();
        	//give some time for the ATX to startup
            delay(1000);
            
            startSong();
        }
        //targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, payload + 1);
        
    }
    else if (btnR == LOW)
    {
        if (DEBUG_SERIAL)
        {
            Serial.println(F("BtnR working"));
            Serial.println(filePos);
        }
    	//NEXT SONG
    	if(filePos >= tracksNum)
    	{
            filePos = 0;
    	}
        else
        {
            filePos += 1;
        }
        //targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, payload + 2);
        delay(200);
        if (DEBUG_SERIAL)
        {
            Serial.println(F("After"));
            Serial.println(filePos);
        }
    }
    else
    {
        floppyCheck();
        //Serial.println(F("Nothing pressed"));
    }
    
    //targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
}

void MoppySD::debugPlay()
{
    myFile = SD.open(F("/64BTET.fm"));
    //   
    //   
    myFile.read(&fmHeader, 5);
    if (DEBUG_SERIAL)
    {
        Serial.println(fmHeader.sizeHeader);
        Serial.println(fmHeader.blockWidth);
        Serial.println(fmHeader.baseDuration);
        Serial.println(fmHeader.notesCount);
    }

    myFile.read(&fmBlock, 256);
    if (DEBUG_SERIAL)
    {
        Serial.println(F("Duration1: "));
        Serial.println(fmBlock.duration1[0]);
        Serial.println(fmBlock.duration1[1]);
        Serial.println(fmBlock.duration1[2]);
        Serial.println(fmBlock.duration1[3]);
    }
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
        targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, (fmBlock.note1 + i));
        //instrument->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note2 + i);
        //delay(tetrisDurationFloppy1[i]*4);
        delay(fmHeader.baseDuration * fmBlock.duration1[i] * 2);
        targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
        //blinkLEDMain();
        delay(50);
    }
}

void MoppySD::floppyCheck()
{
	if(isNowPlaying == true)
	{
        if (messagePos1 < fmHeader.notesCount || messagePos2 < fmHeader.notesCount)
        {
            uint32_t timeElapsed1 = millis() - floppy1Time;
            uint32_t timeElapsed2 = millis() - floppy2Time;
            
            uint32_t delay1 = (fmBlock.duration1[messagePos1] * fmHeader.baseDuration * 2);
            uint32_t delay2 = (fmBlock.duration2[messagePos2] * fmHeader.baseDuration * 2);
            //Serial.println("Floppy1Time: ");
            //Serial.println(timeElapsed1);
            //Serial.println("Floppy2Time: ");
            //Serial.println(timeElapsed2);
            //Serial.println("Delay1: ");
            //Serial.println(delay1);
            //Serial.println("Delay2: ");
            //Serial.println(delay2);
            if(timeElapsed1 > delay1)
            {
                messagePos1++;
                //targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
                targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note1 + messagePos1);

                floppy1Time = millis();
            }
        	if(timeElapsed2 > delay2)
        	{
                messagePos2++;
                //targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
                targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, fmBlock.note2 + messagePos2);
                floppy2Time = millis() - 10;
        	}
        }
        else
        {
            // we should stop
            targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
            targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
            isNowPlaying = false;
            disableATX();
        }
	}
    else
    {
        targetConsumer->handleDeviceMessage(LEAD_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
        targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
    }
}
