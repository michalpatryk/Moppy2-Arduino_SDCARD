#ifndef CUSTOM_SONG_TABLES_H_
#define CUSTOM_SONG_TABLES_H_


#define _OCTAVE (3 * 12)		// makro
#define _C    (_OCTAVE + 0)									//36
#define _c    (12 + _C)										//48
#define _CS   (_OCTAVE + 1)									//37
#define _D    (_OCTAVE + 2)									//38
#define _d    (12 + _D)										//50
#define _DS   (_OCTAVE + 3)									//39
#define _E    (_OCTAVE + 4)									//40
#define _e    (12 + _E)										//52
#define _F    (_OCTAVE + 5)									//41
#define _f    (12 + _F)										//53
#define _FS   (_OCTAVE + 6)									//42
#define _G    (_OCTAVE + 7)									//43
#define _g    (12 + _G)										//55
#define _GS   (_OCTAVE + 8)									//44
#define _A    (_OCTAVE + 9)									//45
#define _a    (12 + _A				- 12)					//45
#define _AS   (_OCTAVE + 10)								//46
#define _B    (_OCTAVE + 11)								//47
#define _b    (12 + _B)										//59
#define _H    (_OCTAVE + 11			- 12)					//35

#define LEAD_FLOPPY 2
#define ACCOMPANIMENT_FLOPPY 1

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
