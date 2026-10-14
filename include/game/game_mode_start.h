#ifndef GAME_GAME_MODE_START_H
#define GAME_GAME_MODE_START_H

#include "types.h"

/* One entry of gGameModeStartTable, indexed by gNextGameMode: the start
 * function of the game mode the main menu picked, and a group number (0 to
 * 10) that GameMode_StartNext stores in gPlayRecords.unk_10. */
typedef struct GameModeStart {
  void (*start)(void);
  u32 group;
} GameModeStart;

typedef char GameModeStart_size_check[sizeof(GameModeStart) == 8 ? 1 : -1];

extern GameModeStart gGameModeStartTable[];

#endif
