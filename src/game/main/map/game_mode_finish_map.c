#include "types.h"
#include "game/Unk_02080440.h"

void func_021019a4(u32 mapId, int value);
void Campaign_FinishMap(void);
void WarRoom_FinishMap(void);
void Versus_FinishMap(void);
void Survival_FinishMap(void);

/* Runs when a map ends. Flags the map in a per-map bit list (func_021019a4,
 * likely "map played"), then runs the end-of-map handler of the game mode. */
void GameMode_FinishMap(void) {
  func_021019a4(data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId, 1);
  switch (data_027c027c->unk_48[data_027c0284]->gameMode) {
    case GAME_MODE_CAMPAIGN:
      Campaign_FinishMap();
      break;
    case GAME_MODE_WAR_ROOM:
      WarRoom_FinishMap();
      break;
    case GAME_MODE_VERSUS:
      Versus_FinishMap();
      break;
    case GAME_MODE_SURVIVAL:
      Survival_FinishMap();
      break;
  }
}
