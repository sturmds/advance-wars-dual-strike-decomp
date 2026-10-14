#include "types.h"
#include "game/game_mode_start.h"
#include "game/play_records.h"

extern int gNextGameMode;
/* The group column of gGameModeStartTable has its own symbol because it
 * starts at +4; the code loads it from there. */
extern u32 gGameModeStartTableGroup[][sizeof(GameModeStart) / sizeof(u32)];

/* Runs the start function of the game mode that the main menu picked
 * (gNextGameMode) and records its group. */
void GameMode_StartNext(void) {
  void (*start)(void) = gGameModeStartTable[gNextGameMode].start;

  gPlayRecords.unk_10 = gGameModeStartTableGroup[gNextGameMode][0];
  start();
}
