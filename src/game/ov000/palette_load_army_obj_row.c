#include "types.h"
#include "game/Unk_02080440.h"

extern u16 data_0213fd2c[][16];
void Palette_Load(const void* src, u32 offset, u32 size);

/* Loads the palette of a player's army (Player.armyColour, the palette; 32-byte
 * rows in data_0213fd2c) into sprite palette slot `slot` (BG slots 0-15 come
 * first, hence the + 16). */
void Palette_LoadArmyObjRow(int player, int slot) {
  Palette_Load(data_0213fd2c[MAP_STATE->players[player].armyColour - 1],
               ((u32)(slot + 16) << 21) >> 16, 32);
}
