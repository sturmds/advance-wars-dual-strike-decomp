#ifndef GAME_MAIN_LOOP_H
#define GAME_MAIN_LOOP_H

#include "types.h"

/* gMainLoop (DTCM): state of the main loop step MainLoop_RunFrame, which
 * runs once per frame and ends with SVC_WaitVBlankIntr. */
typedef struct MainLoop {
  void (*screenCallbacks[2])(
      void); /* called once per frame on each screen when set */
  void (*vblankCallback)(void); /* called by MainLoop_FinishVBlank when set */
  u8 pad_0c[0x18 - 0x0c];
  u32 frameCount; /* MainLoop_RunFrame adds 1 every frame */
  u8 pad_1c[0x24 - 0x1c];
  int unk_24; /* 0, 1 or 2: set by func_01ff9610, func_01ff95fc and func_01ff94fc */
  u16 prevKeys; /* keys seen last frame, for the reset combo */
} MainLoop;

/* 1 once Battle_ResetTurnState has run, 0 while a battle is set up;
 * MainLoop_MapFrame runs the map state only when it is 1. */
extern u32 gMapGameRunning;

#endif
