#ifndef CUSTOM_SONG_TABLES_H_
#define CUSTOM_SONG_TABLES_H_

#include "MoppySD/SDMacros.h"

uint8_t tetrisDurationFloppy1[] = {
	250, 125, 125, 250, 125, 125, 250, 125,
	125, 250, 125, 125, 250, 125, 125, 250,
	250, 250, 750, 250, 125, 250, 125, 125,
	375, 125, 250, 125, 125, 250, 250, 125,
	125, 250, 250, 500
};
uint8_t tetrisNoteFloppy1[] = {
	_E, _H, _C, _D, _C, _H, _A, _A,
	_C, _E, _D, _C, _H, _C, _D, _E,
	_C, _A, _A, _D, _F, _a, _G, _F,
	_E, _C, _E, _D, _C, _H, _C, _D,
	_E, _C, _A, _A
};
const uint8_t tetrisDurationFloppy2[] = {
	1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000
};
const uint8_t tetrisNoteFloppy2[] = {
	_E, _A, _E, _A, _D, _C, _E, _A
};
#endif
