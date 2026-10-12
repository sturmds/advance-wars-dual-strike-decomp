#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"

/* Ov003Map.marks bits. */
#define MARK_OWNED 1   /* the tile belongs to the selected unit's row */
#define MARK_NEXT_TO 2 /* a tile next to a marked one */
#define MARK_UNIT 4    /* a unit of the current row stands here */

extern Ov003Unit* data_ov003_02369534;
extern Ov003Unit data_ov003_02369538[16];
extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */
extern Unk_027c0280* data_027c0280;

int func_ov003_023622b8(void);
int Ov003_IsCurrentSlotRowOpen(void);
void Ov003_DrawMarksAndUnits(void);
void Mem_Fill32(u32 value, void* dst, u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);

#define MAP data_ov003_02364a88
#define STATE data_ov003_02364928
#define TILE_AT(x, y) ((y) * MAP->width + (x))

/* Fills Ov003Map.marks for drawing. When Ov003_IsCurrentSlotRowOpen() is 1 or
 * Ov003State.unk_1df_0 is clear, every mark is set to 0xff and only BG0, BG3
 * and the sprites are shown. Otherwise it marks the tiles of the row of
 * data_ov003_02369534 (except terrain 21 and 26) and the tiles next to them,
 * then the tiles of the current slot's units and the tiles next to those, and
 * shows all layers. */
void Ov003_MarkRowTiles(void) {
  Ov003Unit* unit;
  int x;
  int y;
  int i;

  unit = data_ov003_02369538;
  if (Ov003_IsCurrentSlotRowOpen() == 1 || !STATE->unk_1df_0) {
    Mem_Fill32(-1, MAP->marks, OV003_MAP_TILE_MAX);
    GFX_DISPCNT(data_027c0280)->bg0 = 1;
    GFX_DISPCNT(data_027c0280)->bg1 = 0;
    GFX_DISPCNT(data_027c0280)->bg2 = 0;
    GFX_DISPCNT(data_027c0280)->bg3 = 1;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    return;
  }
  Mem_Zero32(0, MAP->marks, OV003_MAP_TILE_MAX);
  for (y = 0; y < MAP->height; y++) {
    for (x = 0; x < MAP->width; x++) {
      u8 tile = data_ov003_02365d10[MAP->cells[TILE_AT(x, y)]];

      if (data_ov003_02369534->row != ((tile >> 5) & 7) || (tile & 31) == 21 ||
          (tile & 31) == 26) {
        continue;
      }
      MAP->marks[TILE_AT(x, y)] |= MARK_OWNED;
      if (x > 0) {
        MAP->marks[TILE_AT(x - 1, y)] |= MARK_NEXT_TO;
      }
      if (y > 0) {
        MAP->marks[TILE_AT(x, y - 1)] |= MARK_NEXT_TO;
      }
      if (x < MAP->width - 1) {
        MAP->marks[TILE_AT(x + 1, y)] |= MARK_NEXT_TO;
      }
      if (y < MAP->height - 1) {
        MAP->marks[TILE_AT(x, y + 1)] |= MARK_NEXT_TO;
      }
    }
  }
  for (i = 0; i < func_ov003_023622b8(); i++, unit++) {
    if (unit->hp == 0 && !unit->unk_17_7) {
      continue;
    }
    if (unit->row != STATE->slots[STATE->currentSlot].row) {
      continue;
    }
    x = unit->x >> 7;
    y = unit->y >> 7;
    MAP->marks[TILE_AT(x, y)] |= MARK_UNIT;
    if (x > 0) {
      MAP->marks[TILE_AT(x - 1, y)] |= MARK_NEXT_TO;
    }
    if (y > 0) {
      MAP->marks[TILE_AT(x, y - 1)] |= MARK_NEXT_TO;
    }
    if (x < MAP->width - 1) {
      MAP->marks[TILE_AT(x + 1, y)] |= MARK_NEXT_TO;
    }
    if (y < MAP->height - 1) {
      MAP->marks[TILE_AT(x, y + 1)] |= MARK_NEXT_TO;
    }
  }
  Ov003_DrawMarksAndUnits();
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 1;
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 1;
}
