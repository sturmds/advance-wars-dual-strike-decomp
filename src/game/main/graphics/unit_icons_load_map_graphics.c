#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/player.h"

void Mem_CopyAuto(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Palette_LoadPlayerArmyBgRow(int player);
u8* UnitIcons_GetTiles(void);
int UnitIcons_GetTileCount(void);
u8* UnitBadges_GetTiles(void);

extern const u16 gArmyPalettes[][16];

#define UNIT_ICON_FIRST_TILE 0x1e1
#define UNIT_BADGE_FIRST_TILE 0x26a
#define UNIT_BADGE_TILES 30

/* Copies the unit icon tiles (UnitIcons_GetTileCount tiles from UnitIcons_GetTiles) to
 * tile 0x1e1 of the tile data at unk_f8, where gUnitIconTileBase points, and
 * 30 tiles from UnitBadges_GetTiles to tile 0x26a. Then loads the army colours of
 * players 1-4 into BG palette rows 12-15 and the current player's army row. */
void UnitIcons_LoadMapGraphics(void) {
  u8* tiles = data_027c0280->unk_f8;

  Mem_CopyAuto(UnitIcons_GetTiles(),
               tiles + UNIT_ICON_FIRST_TILE * TILE_4BPP_SIZE,
               (u16)(UnitIcons_GetTileCount() * TILE_4BPP_SIZE));
  Mem_CopyAuto(
      UnitBadges_GetTiles(),
      (u8*)data_027c0280->unk_f8 + UNIT_BADGE_FIRST_TILE * TILE_4BPP_SIZE,
      UNIT_BADGE_TILES * TILE_4BPP_SIZE);
  Palette_Load(gArmyPalettes[MAP_STATE->players[1].armyColour - 1], 0x180, 32);
  Palette_Load(gArmyPalettes[MAP_STATE->players[2].armyColour - 1], 0x1a0, 32);
  Palette_Load(gArmyPalettes[MAP_STATE->players[3].armyColour - 1], 0x1c0, 32);
  Palette_Load(gArmyPalettes[MAP_STATE->players[4].armyColour - 1], 0x1e0, 32);
  Palette_LoadPlayerArmyBgRow(MAP_STATE->currentPlayer);
}
