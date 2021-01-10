#include <Arduino.h>
#include "MoppySD.h"
#include "SDMacros.h"
MoppySD::MoppySD(MoppyMessageConsumer* messageConsumer)
{
	targetConsumer = messageConsumer;
}

void MoppySD::enableATX()
{
	Serial.println(F("Enabling ATX"));
	digitalWrite(ATX, HIGH);
}
void MoppySD::loopEvent()
{
    int btnL = digitalRead(BTN_L);
    int btnM = digitalRead(BTN_M);
    int btnR = digitalRead(BTN_R);
    uint8_t payload[] = { _C , _D, _E };
    if (btnL == LOW)
    {
        Serial.println(F("BtnL working"));
        targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, payload);
    }
    else if (btnM == LOW)
    {
        Serial.println(F("BtnM working"));
        targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, payload + 1);
    }
    else if (btnR == LOW)
    {
        Serial.println(F("BtnR working"));
        targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEON, payload + 2);
    }
    else
    {
        //Serial.println(F("Nothing pressed"));
    }
    delay(100);
    targetConsumer->handleDeviceMessage(ACCOMPANIMENT_FLOPPY, NETBYTE_DEV_NOTEOFF, nullptr);
}