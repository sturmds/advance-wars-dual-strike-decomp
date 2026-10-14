#include "types.h"
#include "game/rng.h"
#include "game/battle_save.h"
#include "game/screen_other_map.h"

extern u8 data_02291774[];
extern u8 gGameFlagsLow[];
extern BattleSaveSkillSet
    data_022a72c8[][2]; /* skills a tag partner lends, per player and CO slot */
extern BattleSaveBlock40 data_022a7b98;

void BattleSave_LoadMap(BattleSaveMap* save);
void ForceRank_LoadSets(int mode);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Applies the battle save record in data_02291774 to the game: the map on
 * the current screen and, in a two-map battle, the map on the other screen
 * (BattleSave_LoadMap), then the RNG state, the low game flags, the event
 * script state, data_022a7b98, the Force Rank sets of the game mode and the
 * skills tag partners lend. Undoes BattleSave_Pack. */
void BattleSave_Unpack(void) {
  BattleSaveRecord* record = (BattleSaveRecord*)data_02291774;
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;
  int secondMapId;
  int slot;
  int i;
  int player;

  data_027c0284 = record->screen;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  BattleSave_LoadMap(&record->maps[0]);
  secondMapId = STATE->secondMapId;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);

  if (secondMapId != 0) {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    saved = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    BattleSave_LoadMap(&record->maps[1]);
    Screen_SetCurrent(saved);
    Screen_SetCurrentGfx(savedGfx);
  }

  if (record->unk_00 != 0) {
    STATE->unk_18 = 4;
  }
  gRngState = record->rngState;
  for (i = 0; i < 4; i++) {
    gGameFlagsLow[i] = record->gameFlagsLow[i];
  }
  data_022977a4 = record->eventScript;
  data_022a7b98 = record->unk_98;
  ForceRank_LoadSets(STATE->gameMode);
  for (player = 1; player < 5; player++) {
    /* Needed to match: the original tests the player again. */
    if (player != 0) {
      for (slot = 0; slot < 2; slot++) {
        data_022a72c8[player][slot] = record->lentSkills[player - 1][slot];
      }
    }
  }
}
