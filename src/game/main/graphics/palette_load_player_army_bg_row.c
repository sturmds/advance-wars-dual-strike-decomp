#include "types.h"
#include "game/Unk_02080440.h"

extern const u16 gArmyPalettes[][16];
void Palette_Load(const void* src, u32 offset, u32 size);

/* Loads a player's army palette into background palette slot 11 (offset
 * 0x160). It takes row armyColour + 4 of gArmyPalettes, past the five rows
 * Player_GetPalette uses (the table holds more than five rows). */
void Palette_LoadPlayerArmyBgRow(int player) {
  Palette_Load(gArmyPalettes[MAP_STATE->players[player].armyColour + 4], 0x160,
               32);
}
