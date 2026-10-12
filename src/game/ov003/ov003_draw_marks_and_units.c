#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

extern Ov003Unit data_ov003_02369538[16];
extern u8 data_ov003_02365270[];
extern u8 data_ov003_0236528c[];
extern u8 data_ov003_023652a8[];
extern u8 data_ov003_023652c4[];
extern u8 data_ov003_023652e0[];
extern u8 data_ov003_02365314[];
extern u8 data_ov003_02365348[];
extern u8 data_ov003_0236537c[];
extern u8 data_ov003_023653b0[];
extern u8 data_ov003_023653fc[];
extern u8 data_ov003_02365448[];
extern u8 data_ov003_02365494[];

void Ov003_PutOwnedWindowPiece(int x, int y);
int func_ov003_023622b8(void);
u16 Wireless_GetChildAids(void);
void Oam_PutClippedCells(int x, int y, int a, int b, const void* data, int c,
                         int size);

#define MAP data_ov003_02364a88
#define STATE data_ov003_02364928

#define DRAW(data, size)                                                      \
  Oam_PutClippedCells(x - STATE->scrollX, y - STATE->scrollY, 0, 0x800, data, \
                      0, size)

/* Draws the tile marks (Ov003_PutOwnedWindowPiece) of every 2 by 2 tile block that
 * is on screen or within 47 pixels of it, then each unit of the current slot's
 * row. The unit's kind (1-5) and facing (1-4) pick its picture. When
 * data_02160ac0->unk_7c is set there are 8 units, and only unit 0 and those in
 * the mask from Wireless_GetChildAids are drawn. */
void Ov003_DrawMarksAndUnits(void) {
  Ov003Unit* unit;
  int x;
  int y;
  int i;

  unit = data_ov003_02369538;
  for (y = 0; y < MAP->height; y += 2) {
    if (y * 16 < STATE->scrollY - 47 || y * 16 > STATE->scrollY + 208) {
      continue;
    }
    for (x = 0; x < MAP->width; x += 2) {
      if (x * 16 >= STATE->scrollX - 47 && x * 16 <= STATE->scrollX + 272) {
        Ov003_PutOwnedWindowPiece(x, y);
      }
    }
  }

  for (i = 0; i < func_ov003_023622b8(); i++, unit++) {
    if (data_02160ac0->unk_7c != 0 &&
        !((Wireless_GetChildAids() | 1) & (1 << i))) {
      continue;
    }
    if (unit->hp == 0 && !unit->unk_17_7) {
      continue;
    }
    if (unit->row != STATE->slots[STATE->currentSlot].row) {
      continue;
    }
    /* 128 units per tile, 16 pixels per tile */
    x = unit->x >> 3;
    y = unit->y >> 3;
    switch (unit->kind) {
      case 5:
        switch (unit->facing) {
          case 1:
            DRAW(data_ov003_02365270, 384);
            break;
          case 2:
            DRAW(data_ov003_0236528c, 384);
            break;
          case 3:
            DRAW(data_ov003_023652c4, 384);
            break;
          case 4:
            DRAW(data_ov003_023652a8, 384);
            break;
        }
        break;
      case 1:
      case 4:
        switch (unit->facing) {
          case 1:
            DRAW(data_ov003_0236537c, 512);
            break;
          case 2:
            DRAW(data_ov003_023652e0, 512);
            break;
          case 3:
            DRAW(data_ov003_02365314, 512);
            break;
          case 4:
            DRAW(data_ov003_02365348, 512);
            break;
        }
        break;
      case 2:
      case 3:
        switch (unit->facing) {
          case 1:
            DRAW(data_ov003_023653fc, 784);
            break;
          case 2:
            DRAW(data_ov003_023653b0, 784);
            break;
          case 3:
            DRAW(data_ov003_02365448, 784);
            break;
          case 4:
            DRAW(data_ov003_02365494, 784);
            break;
        }
        break;
    }
  }
}
