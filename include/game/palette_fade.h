#ifndef GAME_PALETTE_FADE_H
#define GAME_PALETTE_FADE_H

#include "types.h"
#include "game/task.h"

/* PaletteFade_Start starts a task that darkens palette slots (16-color
 * palettes, slotCount of them from firstSlot) over `duration` frames. The
 * darkness goes from fromLevel to toLevel, 0 (unchanged) to 16 (black).
 * `screen` picks the screen (0 or 1 in data_027c0288). With fromCurrent
 * nonzero the slots' current colors are what fades; with zero the slots go
 * black at once and `palettes` is what they fade to. parentOrList is
 * Task_Create's second argument. The wrappers below fix some arguments. */
Task* PaletteFade_Start(void* parentOrList, int fromCurrent,
                        const void* palettes, int screen, int duration,
                        int firstSlot, int slotCount, int fromLevel,
                        int toLevel);

/* Fades the slots' current colors from fromLevel to toLevel on screen 0. */
Task* PaletteFade_StartLevels(void* parentOrList, int duration, int firstSlot,
                              int slotCount, int fromLevel, int toLevel);

/* Fades the slots' current colors to black on screen 0. */
Task* PaletteFade_StartToBlack(void* parentOrList, int duration, int firstSlot,
                               int slotCount);

/* Fades the slots' current colors to black on screen 1. */
Task* PaletteFade_StartToBlackScreen1(void* parentOrList, int duration,
                                      int firstSlot, int slotCount);

/* Fades the slots from black to the colors in palettes on screen 1. */
Task* PaletteFade_StartFromBlackScreen1(void* parentOrList,
                                        const void* palettes, int duration,
                                        int firstSlot, int slotCount);

/* Fades the slots from black to the colors in palettes on screen 0. */
Task* PaletteFade_StartFromBlack(void* parentOrList, const void* palettes,
                                 int duration, int firstSlot, int slotCount);

/* Lightens slotCount palettes from firstSlot toward white by level (0 to 16). */
void Palette_Lighten(void* palettes, int firstSlot, int slotCount, int level);

/* Darkens slotCount palettes from firstSlot by level (0 to 16). */
void Palette_Darken(u8* palettes, int firstSlot, int slotCount, int level);

/* Fields of the task PaletteFade_Start creates: it darkens a run of 16-color
 * BG palettes of one screen from one level to another (0 none, 16 black). */
typedef struct PaletteFadeTask {
  u8 pad_00[0x30];
  int active; /* 1 while the fade runs, 0 once done */
  int step;   /* 0-1 fading, 2 frees colors, 3 done */
  u8 pad_38[0x3c - 0x38];
  int frame;  /* starts at 0 */
  int frames; /* length of the fade */
  int firstPalette;
  int paletteCount;
  int startLevel;
  int endLevel;
  int screen; /* 0 or 1, index into data_027c0288 */
  u8 pad_58[0x64 - 0x58];
  u8* palettes; /* first faded palette in the screen's paletteBuffer */
  u8* colors;   /* undarkened colors, paletteCount * 32 bytes */
} PaletteFadeTask;

#endif
