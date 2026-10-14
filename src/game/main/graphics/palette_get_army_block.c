#include "types.h"
#include "game/Unk_02088d04.h"

extern u8 data_0214c79c[];

u8 Player_GetArmyStyle(int player);

/* Returns the 256-byte block of the player's army style; odd and even players use the style's two blocks. */
u8* Palette_GetArmyBlock(int player) {
  int block = ((player - 1) & 1) + (Player_GetArmyStyle(player) - 1) * 2;

  return data_0214c79c + block * PALETTE_BLOCK_SIZE;
}
