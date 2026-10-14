#include "types.h"
#include "game/player.h"
#include "game/rng.h"
#include "game/battle_save.h"
#include "game/screen_other_map.h"
#include "game/save_progress.h"

extern SaveRecordBuffer data_02291774;
extern u8 gGameFlagsLow[];
extern BattleSaveSkillSet
    data_022a72c8[][2]; /* skills a tag partner lends, per player and CO slot */
extern BattleSaveBlock40 data_022a7b98;

void BattleSave_StoreMap(BattleSaveMap* save);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Packs the battle into data_02291774 as one battle save record: flag (as 0
 * or 1), the RNG state, the low game flags, the event script state,
 * data_022a7b98, the current screen, the map on each screen
 * (BattleSave_StoreMap) and the skills tag partners lend. BattleSave_Unpack
 * reverses it. */
void BattleSave_Pack(u8 flag) {
  /* needed to match: through .battle the compiler folds field offsets into
   * the address of data_02291774 */
  BattleSaveRecord* record = (BattleSaveRecord*)&data_02291774;
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;
  int secondMapId;
  int slot;
  int i;
  int player;

  if (flag) {
    record->unk_00 = 1;
  } else {
    record->unk_00 = 0;
  }
  record->rngState = gRngState;
  for (i = 0; i < GAME_FLAGS_LOW_SIZE; i++) {
    record->gameFlagsLow[i] = gGameFlagsLow[i];
  }
  record->eventScript = data_022977a4;
  record->unk_98 = data_022a7b98;
  record->screen = data_027c0284;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  BattleSave_StoreMap(&record->maps[0]);
  secondMapId = STATE->secondMapId;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);

  if (secondMapId != 0) {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    saved = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    BattleSave_StoreMap(&record->maps[1]);
    Screen_SetCurrent(saved);
    Screen_SetCurrentGfx(savedGfx);
  }

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    /* Needed to match: the original tests the player again. */
    if (player != 0) {
      for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
        record->lentSkills[player - 1][slot] = data_022a72c8[player][slot];
      }
    }
  }
}
