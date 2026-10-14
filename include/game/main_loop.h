#ifndef GAME_MAIN_LOOP_H
#define GAME_MAIN_LOOP_H

#include "types.h"

/* gMainLoop (DTCM): state of the main loop step MainLoop_RunFrame, which
 * runs once per frame and ends with SVC_WaitVBlankIntr. */
typedef struct MainLoop {
  void (*screenCallbacks[2])(
      void); /* called once per frame on each screen when set */
  void (*vblankCallback)(void); /* called by MainLoop_FinishVBlank when set */
  int unk_0c;                   /* cleared by func_01ff9ee4 */
  int unk_10;                   /* set to 1 by func_01ff9ed0 */
  int unk_14;                   /* cleared by func_01ff9ba0 */
  u32 frameCount;               /* MainLoop_RunFrame adds 1 every frame */
  int unk_1c; /* func_01ff9f2c adds 1, func_01ff9f54 sets it */
  int unk_20;
  int unk_24; /* 0, 1 or 2: set by func_01ff9610, func_01ff95fc and func_01ff94fc */
  u16 prevKeys; /* keys seen last frame, for the reset combo */
} MainLoop;

/* 1 once Battle_ResetTurnState has run, 0 while a battle is set up;
 * MainLoop_MapFrame runs the map state only when it is 1. */
extern u32 gMapGameRunning;

#endif
