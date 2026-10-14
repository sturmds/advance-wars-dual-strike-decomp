#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290118.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_02290118 data_02290118;

/* 1 when the campaign is played in hard mode, else 0. Map_LoadData and
 * Map_LoadPredeployedUnits use it to pick hardMapData and hardUnits. Only
 * game mode 0 reads the flag; the name assumes mode 0 is the campaign. */
int Campaign_IsHardMode(void) {
  if (data_027c027c->unk_48[data_027c0284]->gameMode == GAME_MODE_CAMPAIGN) {
    return data_02290118.hardMode;
  }
  return 0;
}
