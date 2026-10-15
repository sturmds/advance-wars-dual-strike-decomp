#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"

/* Adds one sprite to draw list `list`; attr holds the tile and palette. */
void Sprite_Queue(int list, int x, int y, void* shape, int attr, int arg5);

extern void*
    gObjShapeBySize[][17]; /* OBJ shape for a sprite of [height][width] tiles */

/* Draws icon iconId at x, y if its bank has it loaded; tile is added to the
 * icon's tile number. The OBJ palette number (bits 12-15 of the attribute)
 * is the bank's palette slot minus 16. */
void Icon_Draw(int iconId, int x, int y, int tile, int list) {
  IconBank* bank;
  int i;

  bank = &data_027c0280->iconBanks[Icon_GetBank(iconId)];
  for (i = 0; i < bank->count; i++) {
    if (iconId == bank->slots[i].iconId) {
      Sprite_Queue(
          list, x, y,
          gObjShapeBySize[gIconHeights[iconId][0]][gIconSize[iconId][0]],
          tile + bank->slots[i].tile + (bank->palette - 16) * 0x1000, 0);
      return;
    }
  }
}
