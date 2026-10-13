#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/unit_list_task.h"

void* Mem_Alloc(u32 size);
void Mem_Free(void* block);
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);
s32 HwDiv32(s32 a, s32 b);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 data_ov000_022b3850
    []; /* indexed by unit type; here walked from 1 to give the list's type order */

#define GAME (data_027c027c->unk_48[data_027c0284])

#define UNIT_LIST_SIZE 64

enum UnitListSort {
  UNIT_LIST_SORT_TYPE,
  UNIT_LIST_SORT_HP,
  UNIT_LIST_SORT_FUEL,
  UNIT_LIST_SORT_AMMO
};

/* Fills the unit list (gUnitListRows) with the current player's units: by
 * type first, then stable-sorted from high to low by shown HP, fuel or ammo,
 * reversed when asked. Last, each carried unit is moved to just after its
 * transport, and a carried transport's own cargo after it.
 *
 * Needed to match: cargoUnit also walks the HP sort, and step counts the
 * fuel levels, the reverse copy and the cargo slots. */
void UnitList_Build(int sortMode, int reverse) {
  int i;
  UnitStats* stats;
  int level;
  int j;
  int count;
  int value;
  MapUnit* unit;
  int step;
  int innerSlot;
  MapUnit* cargoUnit;
  int k;
  int n;
  int m;
  int slot;
  int order;
  u8* sorted;

  sorted = Mem_Alloc(UNIT_LIST_SIZE);
  count = 0;
  for (order = 1; order < UNIT_TYPE_COUNT; order++) {
    for (slot = Ov000_gPlayerFirstUnitSlot[GAME->currentPlayer] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[GAME->currentPlayer] + 51; slot++) {
      value = GAME->units[slot].type;
      if (value != UNIT_NONE && value == data_ov000_022b3850[order]) {
        gUnitListRows[count++] = slot - GAME->currentUnitBase;
      }
    }
  }
  n = 0;
  gUnitListRows[count] = 0;

  switch (sortMode) {
    case UNIT_LIST_SORT_TYPE:
      Mem_CopyUnits(gUnitListRows, sorted, UNIT_LIST_SIZE, 8);
      break;
    case UNIT_LIST_SORT_HP:
      for (level = 10; level > 0; level--) {
        for (i = 0; i < count; i++) {
          cargoUnit = &GAME->units[GAME->currentUnitBase + gUnitListRows[i]];
          if (cargoUnit->hp != 0) {
            value = HwDiv32(cargoUnit->hp - 1, 10) + 1;
          } else {
            value = 0;
          }
          if (level == value) {
            sorted[n++] = gUnitListRows[i];
          }
        }
      }
      break;
    case UNIT_LIST_SORT_FUEL:
      for (step = 99; step >= 0; step--) {
        for (i = 0; i < count; i++) {
          value = GAME->units[GAME->currentUnitBase + gUnitListRows[i]].fuel;
          if (value == step) {
            sorted[n++] = gUnitListRows[i];
          }
        }
      }
      break;
    case UNIT_LIST_SORT_AMMO:
      for (level = 10; level >= -1; level--) {
        for (i = 0; i < count; i++) {
          unit = &GAME->units[GAME->currentUnitBase + gUnitListRows[i]];
          stats = &Ov000_gUnitStats[unit->type];
          /* a type without ammo sorts first if unk_17 is set, else last */
          if (stats->ammo == 0) {
            value = stats->unk_17 != 0 ? 10 : -1;
          } else {
            value = unit->ammo;
          }
          if (value == level) {
            sorted[n++] = gUnitListRows[i];
          }
        }
      }
      break;
  }

  if (reverse) {
    for (step = 0; step < count; step++) {
      gUnitListRows[step] = sorted[count - 1 - step];
    }
    Mem_CopyUnits(gUnitListRows, sorted, UNIT_LIST_SIZE, 8);
  }

  /* Rebuild the list, taking rows out of sorted as they are placed. */
  n = 0;
  for (i = 0; i < count; i++) {
    if (sorted[i] == 0) {
      continue;
    }
    unit = &GAME->units[GAME->currentUnitBase + sorted[i]];
    if (unit->flags & MAP_UNIT_CARRIED) {
      continue;
    }
    gUnitListRows[n++] = sorted[i];
    sorted[i] = 0;
    if (!(unit->flags & MAP_UNIT_CARRYING)) {
      continue;
    }
    for (step = 0; step < 2; step++) {
      if (unit->cargo[step] == 0) {
        continue;
      }
      value = unit->cargo[step] - GAME->currentUnitBase;
      for (j = 0; j < count; j++) {
        if (value == sorted[j]) {
          cargoUnit = &GAME->units[value + GAME->currentUnitBase];
          gUnitListRows[n++] = sorted[j];
          sorted[j] = 0;
          if (cargoUnit->flags & MAP_UNIT_CARRYING) {
            for (k = 0; k < 2; k++) {
              if (cargoUnit->cargo[k] == 0) {
                continue;
              }
              innerSlot = cargoUnit->cargo[k] - GAME->currentUnitBase;
              for (m = 0; m < count; m++) {
                if (innerSlot == sorted[m]) {
                  gUnitListRows[n++] = sorted[m];
                  sorted[m] = 0;
                  break;
                }
              }
            }
          }
          break;
        }
      }
    }
  }
  Mem_Free(sorted);
}
