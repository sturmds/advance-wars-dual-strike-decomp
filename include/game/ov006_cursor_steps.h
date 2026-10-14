#ifndef GAME_OV006_CURSOR_STEPS_H
#define GAME_OV006_CURSOR_STEPS_H

#include "types.h"

/* Per-button cursor steps of the Design Room entry grid (added to the
 * button id), one table per d-pad direction and mode. Ov006_EntryButtonKeys
 * copies a table to the stack before use; the sizes are those of the
 * copies. */
typedef struct CursorSteps18 {
  s8 step[18];
} CursorSteps18;
typedef struct CursorSteps24 {
  s8 step[24];
} CursorSteps24;
typedef struct CursorSteps25 {
  s8 step[25];
} CursorSteps25;
typedef struct CursorSteps26 {
  s8 step[26];
} CursorSteps26;

extern CursorSteps18 data_ov006_0236b700;
extern CursorSteps18 data_ov006_0236b714;
extern CursorSteps24 data_ov006_0236b728;
extern CursorSteps24 data_ov006_0236b740;
extern CursorSteps25 data_ov006_0236b758;
extern CursorSteps25 data_ov006_0236b774;
extern CursorSteps25 data_ov006_0236b790;
extern CursorSteps25 data_ov006_0236b7ac;
extern CursorSteps26 data_ov006_0236b7c8;
extern CursorSteps26 data_ov006_0236b800;
extern CursorSteps26 data_ov006_0236b81c;
extern CursorSteps26 data_ov006_0236b838;

#endif
