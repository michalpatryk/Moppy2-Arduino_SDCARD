/*
 * MoppySD.h
 *
 */
#ifndef SRC_MOPPYNETWORKS_MOPPYSD_H_
#define SRC_MOPPYNETWORKS_MOPPYSD_H_
#include "../MoppyMessageConsumer.h"
#include <SPI.h>
#include <SD.h>
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


class MoppySD {
public:
    MoppySD(MoppyMessageConsumer* messageConsumer);
    void enableATX();
    void disableATX();
    void startupSD();
    
    void loopEvent();
    void debugPlay();
private:
    bool isNowPlaying = false;
    uint8_t filePos = 0;
    uint8_t tracksNum = 2;      // 2 -> 3 tracks on sd card
    uint16_t messagePos1 = 0;    // position on note1/duration1
    uint16_t messagePos2 = 0;    // position on note2/duration2
    uint32_t timer = 0;         // last milis check
    uint32_t floppy1Time = 0;   // 
    uint32_t floppy2Time = 0;
    MoppyMessageConsumer* targetConsumer;
    File myFile;
    FMHeader fmHeader;
    FMBlock fmBlock;
    void startSong();
    void floppyCheck();
};
#endif