#include "types.h"
#include "game/Unk_02088d04.h"

extern u8 data_0214a79c[];

u8* Palette_GetArmyBlock(int player);

/* Returns the 256-byte palette block `index` of the table. Index 8 depends on
 * the player: Palette_GetArmyBlock picks the block from the player's army style. */
u8* Palette_GetBlock(int index, int player) {
  if (index != 8) {
    return data_0214a79c + index * PALETTE_BLOCK_SIZE;
  }
  return Palette_GetArmyBlock(player);
}
