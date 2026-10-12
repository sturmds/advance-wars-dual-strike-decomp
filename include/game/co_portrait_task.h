#ifndef GAME_CO_PORTRAIT_TASK_H
#define GAME_CO_PORTRAIT_TASK_H

#include "types.h"

/* Fields of the task of script data_021613d8 (started by CombatModeCoPortrait_Start):
 * the CO portrait beside the Combat mode menu. When another slot is
 * selected, the portrait slides out, the new CO's graphics load, and it
 * slides back in. The first 0x2a bytes belong to the task system
 * (Task_Create). */
typedef struct CoPortraitTask {
  u8 pad_00[0x2a];
  u16 frame;       /* 0 to CO_PORTRAIT_SLIDE_FRAMES */
  u16 waitFrames;  /* frames spent hidden before the new CO loads */
  s16 slideFrom;   /* offset when the current slide started */
  s16 offset;      /* 0: in place, CO_PORTRAIT_HIDDEN: out of view */
  u8 selectedSlot; /* menu slot selected; index into gCombatModeCos */
  u8 shownSlot;    /* slot whose CO graphics are loaded */
  u8 slidingIn;    /* 1: sliding in or in place, 0: sliding out or hidden */
} CoPortraitTask;

#define CO_PORTRAIT_SLIDE_FRAMES 8
#define CO_PORTRAIT_HIDDEN 144

#endif
