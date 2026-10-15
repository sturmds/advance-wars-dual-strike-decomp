#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/map_unit_marks.h"
#include "game/skills.h"
#include "game/unit_stats.h"
#include "game/math.h"

BOOL UnitSlot_IsVisible(u16 slot);
BOOL UnitType_HidesCargo(int type, int viewer, int owner);
void Map3d_DrawUnit(VecFx32* pos, int type, int ownerBits, int shownHp,
                    u32 marks, int highlight, int stateIs8);

/* needed to match: the first read loads the index before the base. */
#define STATE_INDEX_FIRST                                                 \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

/* Player slot of a unit from its slot in Unk_02080440_48.units with the low
 * six bits cleared (or not): slot 0 is neutral. */
#define SLOT_OWNER(slot) (((slot) >> 6) + 1)

/* Draws the unit on tile (x, y) of the 3D map at pos, with its marks:
 * nothing when the tile has no unit, the tile is hidden, the unit is not
 * visible or it is LOCKED. highlight and stateIs8 (see Map3d_DrawTiles) go on to the draw. */
void Map3d_DrawTileUnit(VecFx32* pos, u16 x, u16 y, int highlight,
                        int stateIs8) {
  BOOL lowFuel;
  int index;
  int slot;
  MapUnit* unit;
  u8 maxFuel;
  int ownerBits;
  MapTiles* tiles;
  BOOL lowAmmo;
  u32 marks;
  u8 maxAmmo;
  int shownHp;

  tiles = STATE_INDEX_FIRST->mapTiles;
  index = x + tiles->rowStart[y];
  slot = tiles->unk_508[index];
  if (slot == 0) {
    return;
  }
  if (tiles->unk_2338[index] == 0) {
    return;
  }
  if (!UnitSlot_IsVisible(slot)) {
    return;
  }
  unit = &MAP_STATE->units[slot];
  if (MAP_STATE->units[slot].flags & MAP_UNIT_LOCKED) {
    return;
  }
  marks = 0;
  lowFuel = FALSE;
  ownerBits = slot & 0xc0;
  maxFuel = Ov000_gUnitStats[unit->type].fuel;
  if (maxFuel != 0 && unit->fuel <= maxFuel >> 1) {
    lowFuel = TRUE;
  }
  if (lowFuel) {
    marks |= UNIT_MARK_LOW_FUEL;
  }
  lowAmmo = FALSE;
  maxAmmo = Ov000_gUnitStats[unit->type].ammo;
  if (maxAmmo != 0 && unit->ammo <= maxAmmo >> 1) {
    lowAmmo = TRUE;
  }
  if (lowAmmo) {
    marks |= UNIT_MARK_LOW_AMMO;
  }
  if (unit->flags & MAP_UNIT_PARALYSED) {
    marks |= UNIT_MARK_PARALYSED;
  }
  if (unit->flags & MAP_UNIT_DIVED) {
    marks |= UNIT_MARK_DIVED;
  }
  if (!(MAP_STATE->players[SLOT_OWNER(ownerBits)].visionGrids & 2)) {
    if (CheckSkillsEquipped(MAP_STATE->players[SLOT_OWNER(slot)].skills[0],
                            SKILL_HIDDEN_HP)) {
      marks |= UNIT_MARK_HIDDEN_HP;
    }
  }
  if (unit->captureProgress) {
    marks |= UNIT_MARK_CAPTURING;
  }
  if (unit->cargo[0]) {
    marks |= UNIT_MARK_CARRYING;
  }
  if (UnitType_HidesCargo(unit->type, MAP_STATE->currentPlayer,
                          (u16)SLOT_OWNER(ownerBits))) {
    marks |= UNIT_MARK_HIDDEN_CARGO;
  }
  if (MAP_STATE->units[slot].flags & MAP_UNIT_WAITED) {
    marks |= UNIT_MARK_WAITED;
  }
  /* hp 1-100 shows as 1-10 */
  if (unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(unit->hp - 1, 10) + 1;
  }
  Map3d_DrawUnit(pos, unit->type, ownerBits, shownHp, marks, highlight,
                 stateIs8);
}
