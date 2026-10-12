#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);
void Co_QueueLargePictureCopy(int coId, void* dest, int paletteSlot, int army);

extern MenuState gMenuState;
extern u8 data_02160ae0[];

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Link message 19: the row's player picks the next of the seven COs
 * (unk_19 of the row, 0-6, wraps to 0) and the row's picture is loaded
 * again, 36 tiles per row from tile 0x400. A confirmed row ignores it. */
void CombatSetup_NextCo(int player) {
  int row;
  u32 index;
  int offset;
  u8* coIndex;

  row = MENU->combat.players[player].row;
  index = row - 1;

  if (MENU->combat.rows[index].locked != 0) {
    return;
  }
  Sound_PlaySe(0x10002);
  offset = index * sizeof(Unk_02160ac0_Row);
  coIndex = (u8*)data_02160ac0 + 0x19 + offset;
  if (*coIndex < 6) {
    (*coIndex)++;
  } else {
    *coIndex = 0;
  }
  Co_QueueLargePictureCopy(
      data_02160ae0[LINK_ROWS[index].unk_19],
      data_027c0280->objVram + ((row - 1) * 36 + 1024) * 32, row + 17, row);
}
