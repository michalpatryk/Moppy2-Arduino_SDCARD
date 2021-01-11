#ifndef _PIN_DEFINES_H_
#define _PIN_DEFINES_H_

#define ATX 3
#define BTN_R 14
#define BTN_M 15
#define BTN_L 16
#define SS 10

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
#define ACCOMPANIMENT_FLOPPY 3

#define DEBUG_SERIAL true
#endif
