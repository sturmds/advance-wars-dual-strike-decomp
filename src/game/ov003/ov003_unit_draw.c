#include "types.h"
#include "game/ov003_unit.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
int Ov003_RowKindGroup(int a0);
int Ov003_IsPhase0OrAbove6(void);
void Ov003_DrawTexturedQuad(int x, int y, int a2, int a3, int a4, int a5,
                            int a6, int a7, int a8);
void Ov003Unit_DrawMarker(Ov003Unit* unit);

#define STATE data_ov003_02364928

/* Draws a unit that is on screen (position >> 3 is in pixels, like the
 * scroll): its sprite, with the frame picked by walkCounter and
 * mirrored when it faces direction 4, then an hp marker for hp 1 to 90 (one
 * step per 10 hp) and, once unk_17_0 reaches 30, a second marker. Tile kind 0
 * and kinds above 5 draw nothing. */
void Ov003Unit_Draw(Ov003Unit* unit) {
  int base;
  int flip;
  int hpStep;
  int dir;
  int frame;

  flip = 0;
  if (unit->x >> 3 < STATE->scrollX - 15 ||
      unit->x >> 3 > STATE->scrollX + 272) {
    return;
  }
  if (unit->y >> 3 < STATE->scrollY - 15 ||
      unit->y >> 3 > STATE->scrollY + 208) {
    return;
  }
  switch (unit->kind) {
    case 1:
    case 2:
      if (Ov003_RowKindGroup(STATE->rows[unit->row - 1].co) == 5) {
        base = 684;
      } else {
        base = 108;
      }
      break;
    case 3:
      base = 252;
      break;
    case 4:
      base = 396;
      break;
    case 5:
      base = 540;
      break;
    default:
      return;
  }
  dir = unit->facing;
  switch (dir) {
    case 1:
      base += 96;
      break;
    case 2:
      base += 48;
      break;
  }
  if (dir == 4) {
    flip = 1;
  }
  frame = base + HwMod32(unit->walkCounter >> 3, 3) * 16;
  Ov003_DrawTexturedQuad((unit->x >> 3) - STATE->scrollX - 16,
                         (unit->y >> 3) - STATE->scrollY - 16, 20, 2, 2, frame,
                         unit->row + 10, flip, 0);
  if (unit->hp == 0) {
    hpStep = 0;
  } else {
    hpStep = HwDiv32(unit->hp - 1, 10) + 1;
  }
  if (hpStep >= 1 && hpStep <= 9) {
    Ov003_DrawTexturedQuad((unit->x >> 3) - STATE->scrollX,
                           (unit->y >> 3) - STATE->scrollY + 1, 22, 0, 0,
                           hpStep + 828, unit->row + 10, 0, 0);
    if (unit->unk_17_0 >= 30) {
      if ((STATE->frameCount & 16) == 0 || Ov003_IsPhase0OrAbove6() == 0) {
        Ov003_DrawTexturedQuad((unit->x >> 3) - STATE->scrollX - 8,
                               (unit->y >> 3) - STATE->scrollY, 22, 0, 0, 1232,
                               17, 0, 0);
      }
    }
  }
  Ov003Unit_DrawMarker(unit);
}
