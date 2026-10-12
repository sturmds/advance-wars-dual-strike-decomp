#include "types.h"
#include "game/play_records.h"

extern int gNextGameMode;
/* 32 entries of 8 bytes, indexed by gNextGameMode: a start function, then a
 * group number (0 to 10) that goes to gPlayRecords.unk_10. The group column
 * has its own symbol because it starts at +4. */
extern void (*gGameModeStartTable[][2])(void);
extern u32 gGameModeStartTableGroup[][2];

/* Runs the start function of the game mode that the main menu picked
 * (gNextGameMode) and records its group. */
void GameMode_StartNext(void) {
  void (*start)(void) = gGameModeStartTable[gNextGameMode][0];

  gPlayRecords.unk_10 = gGameModeStartTableGroup[gNextGameMode][0];
  start();
}
