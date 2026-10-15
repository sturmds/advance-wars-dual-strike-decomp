#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

/* Byte 4 of the 20-byte Ov003TileInfo entries in data_ov003_02364d88: index
 * with tile * 20. The reach in tiles, minus 1, for func_ov003_0235f8e8. */
extern u8 data_ov003_02364d8c[];

void Ov003Unit_Move(Ov003Unit* unit, int dir, int arg2, int arg3);
int Ov003_GetMoveSpeed(int player, int kind, int terrain);
int Ov003Unit_GetStepDirection(Ov003Unit* unit);
void func_ov003_0235f81c(Ov003Unit* unit);
void func_ov003_0235f8e8(Ov003Unit* unit);
void Ov003Unit_FireIfLocked(Ov003Unit* unit);
u32 Ov003_GetTargetRange(void);
u32 Ov003Unit_FindNearest(Ov003Unit* unit,
                          s8* target); /* distance to target, 16 per tile */
u32 Ov003_RandNext(void);

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88
#define TERRAIN_AT(tile) (data_ov003_02365d10[MAP->cells[tile]] & 31)

/* Points the unit in direction d (1 up, 2 down, 3 left, 4 right). */
#define SET_DIR(unit, d)                           \
  do {                                             \
    (unit)->facing = (d);                          \
    Ov003Unit_Move((unit), (unit)->facing, -1, 1); \
    (unit)->walkCounter++;                         \
  } while (0)

/* Turns the unit towards the centre of its tile (64 of 128) when it is more
 * than one step (Ov003_GetMoveSpeed) away, then runs Ov003Unit_FireIfLocked. */
#define STEP_TO_CENTRE(unit)                                           \
  do {                                                                 \
    int step;                                                          \
    (unit)->unk_1c_0 = 0;                                              \
    step = Ov003_GetMoveSpeed(                                         \
        (unit)->row, (unit)->kind,                                     \
        TERRAIN_AT(((unit)->y >> 7) * MAP->width + ((unit)->x >> 7))); \
    if (((unit)->y & ~127) + 64 < (unit)->y - step) {                  \
      SET_DIR(unit, 1);                                                \
    } else if (((unit)->y & ~127) + 64 > (unit)->y + step) {           \
      SET_DIR(unit, 2);                                                \
    } else if (((unit)->x & ~127) + 64 < (unit)->x - step) {           \
      SET_DIR(unit, 3);                                                \
    } else if (((unit)->x & ~127) + 64 > (unit)->x + step) {           \
      SET_DIR(unit, 4);                                                \
    }                                                                  \
    Ov003Unit_FireIfLocked(unit);                                      \
  } while (0)

/* One frame of a computer unit. It wakes (sets awake) when its wake group
 * wakeGroup is set in Ov003State.unk_1de, when it is hurt, or when its target
 * is closer than wakeRange tiles; waking also sets groups 0 to wakeGroup. A unit
 * that is capturing (unk_04 still counting) or has unk_17_0 set stays at the
 * centre of its tile instead, under the conditions below; the range from
 * Ov003_GetTargetRange is 0 or 8 tiles. Otherwise an awake unit heads for its
 * target every 32 frames when Ov003Unit_GetStepDirection gives a direction, or turns
 * at random every 256 frames and whenever it has not moved since
 * Ov003_SnapshotUnits (blocked). Then a target in reach goes to
 * func_ov003_0235f8e8, anything else to func_ov003_0235f81c. */
void Ov003Unit_UpdateAi(Ov003Unit* unit) {
  u32 dist;
  u32 range;
  u8 dir;

  dist = Ov003Unit_FindNearest(unit, &unit->target);
  range = Ov003_GetTargetRange();

  if (!unit->awake) {
    if (unit->wakeGroup != 0 && (STATE->unk_1de & (1 << unit->wakeGroup))) {
      unit->awake = 1;
    } else if (unit->hp != 100 || dist < ((unit->wakeRange << 7) >> 3)) {
      unit->awake = 1;
      if (unit->wakeGroup != 0) {
        STATE->unk_1de |= ~(0xff << (unit->wakeGroup + 1));
      }
    }
  }
  unit->unk_1c_0 = 0;

  if (unit->unk_04 != 0 &&
      (unit->unk_04 != unit->prevUnk_04 || unit->unk_11 != 0)) {
    if (data_02160ac0->unk_7c == 0 &&
        (unit->unk_1d_6 || TERRAIN_AT(unit->lastTile) == 8)) {
      if (dist >= range ||
          (!STATE->unk_1df_5 &&
           (unit->unk_04 > 30 || TERRAIN_AT(unit->lastTile) == 8))) {
        STEP_TO_CENTRE(unit);
        return;
      }
    }
  } else if (unit->unk_17_0 != 0 && dist >= range) {
    STEP_TO_CENTRE(unit);
    return;
  }

  if (!unit->awake) {
    return;
  }
  dir = 0;
  if (STATE->unk_1df_5 && dist < 128 && unit->target != -1 &&
      (unit->aiTimer & 31) == 0) {
    dir = Ov003Unit_GetStepDirection(unit);
  }
  if (dir != 0) {
    unit->facing = dir;
  } else if (unit->aiTimer == 0) {
    unit->facing = (u8)((Ov003_RandNext() & 3) + 1);
  }
  Ov003Unit_Move(unit, unit->facing, -1, 1);
  if (!unit->unk_1c_0 &&
      (!unit->unk_1c_1 || unit->kind == 5 || !unit->unk_1d_7) &&
      unit->x == unit->prevX && unit->y == unit->prevY) {
    unit->facing = (u8)((Ov003_RandNext() & 3) + 1);
    Ov003Unit_Move(unit, unit->facing, -1, 1);
  }
  if (unit->target == -1 ||
      dist > (((data_ov003_02364d8c[unit->kind * 20] + 1) << 7) >> 3)) {
    func_ov003_0235f81c(unit);
  } else {
    func_ov003_0235f8e8(unit);
  }
  if (unit->shotWait != 0) {
    unit->shotWait--;
  }
  unit->walkCounter++;
  unit->aiTimer++;
}
