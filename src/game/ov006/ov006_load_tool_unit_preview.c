#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/screen_other_map.h"

extern const u16 gArmyPalettes[][16];

void Palette_Load(const void* src, u32 offset, u32 size);
void TransferQueue_Add(const void* src, void* dst, u32 size);
u8* UnitIcons_GetTiles(void);
int Player_GetUnitIconTile(int player, int unitType);
u8* UnitType_GetPictureTiles(int unitType);

#define STATE data_ov006_0236d640

/* Loads the unit type in STATE->toolUnit (low 6 bits) for display on the
 * second screen: its 4-tile icon at 0x5480 in OBJ VRAM (tile index from
 * Player_GetUnitIconTile, row picked by the player's army style, the sprite set), its
 * 8-tile picture (UnitType_GetPictureTiles) at 0x5500, and the player's army colour (the
 * palette) in sprite palette slot 2. STATE->unitArmy is the player; 0 means
 * player 1. */
void Ov006_LoadToolUnitPreview(void) {
  int player = STATE->unitArmy;
  int palette;
  int unitType;
  u8* icons;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (player == 0) {
    player = 1;
  }
  palette = data_027c027c->unk_48[data_027c0284]->players[player].armyColour;
  unitType = STATE->toolUnit & 0x3f;
  icons = UnitIcons_GetTiles();
  TransferQueue_Add(
      icons + Player_GetUnitIconTile(player, unitType) * 32,
      (u8*)((Unk_027c0280*)data_027c0288.unk_04)->objVram + 0x5480, 128);
  TransferQueue_Add(
      UnitType_GetPictureTiles(unitType),
      (u8*)((Unk_027c0280*)data_027c0288.unk_04)->objVram + 0x5500, 256);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Palette_Load(gArmyPalettes[palette - 1], 0x240, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
