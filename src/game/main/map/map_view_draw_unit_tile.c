#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/skills.h"
#include "game/math.h"

void MapView_ClearUnitTile(u16 x, u16 y);
BOOL UnitSlot_IsVisible(int slot);
u16* Bg_GetMapEntry(int layer, int x, int y);
void UnitIcon_DrawEntries(u16* entry, int type, int look, u8 carrying,
                          u8 capture, u16 shownHp, u16 dived, u8 hpHidden,
                          BOOL lowFuel, BOOL lowAmmo, BOOL paralysed);

/* Draws the unit shown on map tile (x, y) into BG layer 1, or clears the
 * tile when there is none to show. look is 0x100 for a unit that has
 * waited (grey), else the slot's owner bits. */
void MapView_DrawUnitTile(int x, int y) {
  u8 slot;
  BOOL flag;
  MapUnit* unit;
  u16 half;
  MapTiles* tiles;
  BOOL lowFuel;
  int shownHp;
  BOOL lowAmmo;
  u16* entry;
  int idx;
  BOOL paralysed;
  int bgX;
  int look;
  int bgY;
  BOOL hpHidden;

  if (x < 0 || x >= MAP_STATE->unk_1c4->mapWidth || y < 0 ||
      y >= MAP_STATE->unk_1c4->mapHeight) {
    MapView_ClearUnitTile(x, y);
    return;
  }
  tiles = STATE_IDX_FIRST->mapTiles;
  idx = x + tiles->rowStart[y];
  slot = tiles->unk_508[idx];
  if (slot == 0 || tiles->unk_2338[idx] == 0 || !UnitSlot_IsVisible(slot) ||
      (MAP_STATE->units[slot].flags & MAP_UNIT_LOCKED)) {
    MapView_ClearUnitTile(x, y);
    return;
  }
  if (MAP_STATE->units[slot].flags & MAP_UNIT_WAITED) {
    look = 0x100;
  } else {
    look = (u16)(slot & 0xc0);
  }
  lowFuel = FALSE;

  bgX = (s16)(((x - MAP_STATE->unk_1c4->unk_26) & 15) * 2);
  bgY = (s16)(((y - MAP_STATE->unk_1c4->unk_28) & 15) * 2);
  lowAmmo = lowFuel;
  paralysed = lowFuel;
  unit = &MAP_STATE->units[slot];

  flag = FALSE;
  if (Ov000_gUnitStats[unit->type].fuel != 0 &&
      unit->fuel <= Ov000_gUnitStats[unit->type].fuel >> 1) {
    flag = TRUE;
  }
  if (flag) {
    lowFuel = TRUE;
  }
  flag = FALSE;
  if (Ov000_gUnitStats[unit->type].ammo != 0 &&
      unit->ammo <= Ov000_gUnitStats[unit->type].ammo >> 1) {
    flag = TRUE;
  }
  if (flag) {
    lowAmmo = TRUE;
  }
  if (unit->flags & MAP_UNIT_PARALYSED) {
    paralysed = TRUE;
  }
  half = ((x - MAP_STATE->unk_1c4->unk_26) & 16) ? 0x400 : 0;
  entry = Bg_GetMapEntry(1, bgX, bgY) + half;
  if (unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(unit->hp - 1, 10) + 1;
  }
  hpHidden = CheckSkillsEquipped(MAP_STATE->players[(slot >> 6) + 1].skills[0],
                                 SKILL_HIDDEN_HP);
  UnitIcon_DrawEntries(entry, MAP_STATE->units[slot].type, look,
                       unit->cargo[0] | unit->cargo[1], unit->captureProgress,
                       shownHp, unit->flags & MAP_UNIT_DIVED, hpHidden, lowFuel,
                       lowAmmo, paralysed);
}
