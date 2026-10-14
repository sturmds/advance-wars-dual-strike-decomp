#include "types.h"
#include "game/ov003_unit.h"

int Ov003_GetMoveSpeed(int player, int kind, int terrain);

extern Ov003Unit data_ov003_02369538[];
extern u8 data_ov003_02365d10[];

#define MAP data_ov003_02364a88
#define TERRAIN(tx, ty) \
  (data_ov003_02365d10[MAP->cells[(ty) * MAP->width + (tx)]] & 31)

/* Moves a unit up to `distance` (its own speed when negative) in direction
 * `dir`. A unit fills a 96 by 96 box around (x, y), 128 units per tile. It
 * stops against other live units and against tiles its kind cannot cross
 * (base speed 0 there). On a straight move (`straight` 1), bumping a unit of
 * another player sets unk_1c_0 and stores that unit's index in lockedTarget
 * (never for kind 5), and stopping at a tile sets unk_1c_1 when the tile ahead
 * of the unit's centre has terrain 3 or 16. */
void Ov003Unit_Move(Ov003Unit* unit, int dir, int distance, int straight) {
  /* needed to match: declaration order */
  Ov003Unit* other;
  int terrain;
  int i;
  s16 left;
  s16 cx;
  s16 right;
  int j;
  int otherLeft;
  s16 top;
  int otherRight;
  BOOL retried;
  int otherBottom;
  s16 bottom;
  int otherTop;
  s16 cy;
  int edge;
  u8 blocked; /* bit 0, bit 1: the two leading corners */

  retried = FALSE;

  for (;;) {
    if (distance < 0) {
      distance = Ov003_GetMoveSpeed(unit->row, unit->kind,
                                    TERRAIN(unit->x >> 7, unit->y >> 7));
    }
    unit->unk_1c_0 = 0;
    unit->unk_1c_1 = 0;

    switch (dir) {
      case OV003_DIR_LEFT:
        left = unit->x - 48 - distance;
        right = unit->x - 48;
        top = unit->y - 48;
        bottom = unit->y + 48;
        break;
      case OV003_DIR_RIGHT:
        left = unit->x + 48;
        right = unit->x + 48 + distance;
        top = unit->y - 48;
        bottom = unit->y + 48;
        break;
      case OV003_DIR_UP:
        left = unit->x - 48;
        right = unit->x + 48;
        top = unit->y - 48 - distance;
        bottom = unit->y - 48;
        break;
      case OV003_DIR_DOWN:
        left = unit->x - 48;
        right = unit->x + 48;
        top = unit->y + 48;
        bottom = unit->y + 48 + distance;
        break;
      default:
        return;
    }

    if (left < 0) {
      left = 0;
    }
    if ((right >> 7) >= MAP->width) {
      right = (MAP->width << 7) - 1;
    }
    if (top < 0) {
      top = 0;
    }
    if ((bottom >> 7) >= MAP->height) {
      bottom = (MAP->height << 7) - 1;
    }

    switch (dir) {
      case OV003_DIR_LEFT:
        edge = left;
        break;
      case OV003_DIR_RIGHT:
        edge = right;
        break;
      case OV003_DIR_UP:
        edge = top;
        break;
      case OV003_DIR_DOWN:
        edge = bottom;
        break;
    }

    other = data_ov003_02369538;
    for (i = 0; i < OV003_UNIT_COUNT; i++, other++) {
      if (other == unit) {
        continue;
      }
      if (other->hp == 0) {
        continue;
      }
      otherRight = other->x + 48;
      if (left > otherRight) {
        continue;
      }
      otherLeft = other->x - 48;
      if (right < otherLeft) {
        continue;
      }
      otherBottom = other->y + 48;
      if (top > otherBottom) {
        continue;
      }
      otherTop = other->y - 48;
      if (bottom < otherTop) {
        continue;
      }
      switch (dir) {
        case OV003_DIR_LEFT:
          if (otherRight < edge) {
            continue;
          }
          edge = other->x + 49;
          break;
        case OV003_DIR_RIGHT:
          if (otherLeft > edge) {
            continue;
          }
          edge = other->x - 49;
          break;
        case OV003_DIR_UP:
          if (otherBottom < edge) {
            continue;
          }
          edge = other->y + 49;
          break;
        case OV003_DIR_DOWN:
          if (otherTop > edge) {
            continue;
          }
          edge = other->y - 49;
          break;
      }
      if (straight == 1) {
        if (other->row == unit->row || unit->kind == 5) {
          unit->unk_1c_0 = 0;
        } else {
          unit->unk_1c_0 = 1;
          unit->lockedTarget = i;
        }
      }
    }

    blocked = 0;
    switch (dir) {
      case OV003_DIR_LEFT:
        for (j = 0; j < 2; j++) {
          switch (j) {
            case 0:
              cx = edge >> 7;
              cy = top >> 7;
              break;
            case 1:
              cx = edge >> 7;
              cy = bottom >> 7;
              break;
          }
          if (Ov003_GetMoveSpeed(0, unit->kind, TERRAIN(cx, cy)) == 0) {
            blocked |= 1 << j;
          }
        }
        if (blocked != 0) {
          /* one corner blocked: slide once toward the free side, then
                 * try the whole move again */
          if (!retried && straight == 1) {
            if (blocked == 1) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_DOWN, distance, 0);
              continue;
            }
            if (blocked == 2) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_UP, distance, 0);
              continue;
            }
          }
          edge = (cx + 1) << 7;
          if (straight == 1) {
            terrain = TERRAIN(cx, unit->y >> 7);
            if (terrain == 3 || terrain == 16) {
              unit->unk_1c_1 = 1;
            } else {
              unit->unk_1c_1 = 0;
            }
          }
        }
        break;
      case OV003_DIR_RIGHT:
        for (j = 0; j < 2; j++) {
          switch (j) {
            case 0:
              cx = edge >> 7;
              cy = top >> 7;
              break;
            case 1:
              cx = edge >> 7;
              cy = bottom >> 7;
              break;
          }
          if (Ov003_GetMoveSpeed(0, unit->kind, TERRAIN(cx, cy)) == 0) {
            blocked |= 1 << j;
          }
        }
        if (blocked != 0) {
          if (!retried && straight == 1) {
            if (blocked == 1) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_DOWN, distance, 0);
              continue;
            }
            if (blocked == 2) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_UP, distance, 0);
              continue;
            }
          }
          edge = (cx << 7) - 1;
          if (straight == 1) {
            terrain = TERRAIN(cx, unit->y >> 7);
            if (terrain == 3 || terrain == 16) {
              unit->unk_1c_1 = 1;
            } else {
              unit->unk_1c_1 = 0;
            }
          }
        }
        break;
      case OV003_DIR_UP:
        for (j = 0; j < 2; j++) {
          switch (j) {
            case 0:
              cx = left >> 7;
              cy = edge >> 7;
              break;
            case 1:
              cx = right >> 7;
              cy = edge >> 7;
              break;
          }
          if (Ov003_GetMoveSpeed(0, unit->kind, TERRAIN(cx, cy)) == 0) {
            blocked |= 1 << j;
          }
        }
        if (blocked != 0) {
          if (!retried && straight == 1) {
            if (blocked == 1) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_RIGHT, distance, 0);
              continue;
            }
            if (blocked == 2) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_LEFT, distance, 0);
              continue;
            }
          }
          edge = (cy + 1) << 7;
          if (straight == 1) {
            terrain = TERRAIN(unit->x >> 7, cy);
            if (terrain == 3 || terrain == 16) {
              unit->unk_1c_1 = 1;
            } else {
              unit->unk_1c_1 = 0;
            }
          }
        }
        break;
      case OV003_DIR_DOWN:
        for (j = 0; j < 2; j++) {
          switch (j) {
            case 0:
              cx = left >> 7;
              cy = edge >> 7;
              break;
            case 1:
              cx = right >> 7;
              cy = edge >> 7;
              break;
          }
          if (Ov003_GetMoveSpeed(0, unit->kind, TERRAIN(cx, cy)) == 0) {
            blocked |= 1 << j;
          }
        }
        if (blocked != 0) {
          if (!retried && straight == 1) {
            if (blocked == 1) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_RIGHT, distance, 0);
              continue;
            }
            if (blocked == 2) {
              retried = TRUE;
              Ov003Unit_Move(unit, OV003_DIR_LEFT, distance, 0);
              continue;
            }
          }
          edge = (cy << 7) - 1;
          if (straight == 1) {
            terrain = TERRAIN(unit->x >> 7, cy);
            if (terrain == 3 || terrain == 16) {
              unit->unk_1c_1 = 1;
            } else {
              unit->unk_1c_1 = 0;
            }
          }
        }
        break;
    }

    switch (dir) {
      case OV003_DIR_LEFT:
        unit->x = edge + 48;
        return;
      case OV003_DIR_RIGHT:
        unit->x = edge - 48;
        return;
      case OV003_DIR_UP:
        unit->y = edge + 48;
        return;
      case OV003_DIR_DOWN:
        unit->y = edge - 48;
        return;
    }
    return;
  }
}
