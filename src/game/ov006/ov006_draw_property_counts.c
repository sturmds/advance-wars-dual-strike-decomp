#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);
void Oam_PutSmallNumber(u16 x, u16 y, int value);
int Ov006_CountPropertyTilesMatching(int tile);

typedef struct RowShapes {
  void* shape[5];
} RowShapes;

typedef struct ColumnShapes {
  void* shape[6];
} ColumnShapes;

typedef struct TileGrid {
  s16 tile[5 * 6];
} TileGrid;

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern Unk_ov006_0235f690* data_ov006_0236d640;
extern RowShapes data_ov006_0236aa70;
extern ColumnShapes data_ov006_0236aa84;
extern TileGrid data_ov006_0236aa9c;
extern u8 data_ov006_0236a9e8[];
extern u8 data_ov006_0236a9f0[];
extern u8 data_ov006_0236aa00[];
extern u8 data_ov006_0236aa18[];
extern u8 data_ov006_0236aa28[];
extern u8 data_ov006_0236aa30[];

#define OV6 data_ov006_0236d640

/* OAM attribute 0 bit 10: semi-transparent, for a count of 0. */
#define DIMMED(count) ((count) != 0 ? 0 : 0x400)

/* Draws the sub screen's count table with sprites: five rows (neutral and
 * the four armies, a row icon in column 0 except for neutral) of five
 * property tiles each, from the tile grid at data_ov006_0236aa9c, each with
 * its count from Ov006_CountPropertyTilesMatching; then the counts of tiles
 * 17 and 16, and the four armies' unit counts (OV6->unk_2c). A sprite whose
 * count is 0 is drawn semi-transparent and gets no number. Tile 14 uses the
 * sprite tiles 32 further on. The declaration order is needed to match. */
void Ov006_DrawPropertyCounts(void) {
  int col;
  int row;
  int y;
  int i;
  ColumnShapes columnShapes;
  int x;
  RowShapes rowShapes;
  TileGrid tiles;
  int palette;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int count;
  int tile;
  int tileOffset;

  columnShapes = data_ov006_0236aa84;
  rowShapes = data_ov006_0236aa70;
  tiles = data_ov006_0236aa9c;

  saved0280 = data_027c0280;
  palette = 4;
  i = 0;
  y = 21;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  for (row = 0; row < 5; row++) {
    x = 136;
    for (col = 0; col < 6; col++) {
      if (col == 0) {
        if (row > 0) {
          count = Ov006_CountPropertyTilesMatching(tiles.tile[i]);
          if (count != 0) {
            Oam_PutSmallNumber((x + 9) & 0x1ff, (y + 25) & 0xff, count);
          }
          Oam_PutSprite(x & 0x1ff, (y & 0xff) | DIMMED(count),
                        rowShapes.shape[row], palette << 12);
        }
      } else {
        tile = tiles.tile[i];
        count = Ov006_CountPropertyTilesMatching(tile);
        if (count != 0) {
          Oam_PutSmallNumber((x + 9) & 0x1ff, (y + 25) & 0xff, count);
        }
        /* needed to match: an if, not a ternary */
        tileOffset = 0;
        if (tile == 14) {
          tileOffset = 32;
        }
        Oam_PutSprite(x & 0x1ff, (y & 0xff) | DIMMED(count),
                      columnShapes.shape[col], (palette << 12) + tileOffset);
      }
      x += 19;
      i++;
    }
    y += 25;
    palette++;
  }

  count = Ov006_CountPropertyTilesMatching(17);
  if (count != 0) {
    Oam_PutSmallNumber(144, (y + 22) & 0xff, count);
  }
  Oam_PutSprite(136, ((y - 2) & 0xff) | DIMMED(count), data_ov006_0236a9f0,
                0x9000);
  count = Ov006_CountPropertyTilesMatching(16);
  if (count != 0) {
    Oam_PutSmallNumber(164, (y + 22) & 0xff, count);
  }
  Oam_PutSprite(156, ((y - 2) & 0xff) | DIMMED(count), data_ov006_0236aa30,
                0xa000);

  count = (s8)OV6->unk_2c[0];
  if (count != 0) {
    Oam_PutSmallNumber(182, (y + 22) & 0xff, count);
  }
  Oam_PutSprite(174, ((y + 10) & 0xff) | DIMMED(count), data_ov006_0236aa00,
                0xb000);
  count = (s8)OV6->unk_2c[1];
  if (count != 0) {
    Oam_PutSmallNumber(202, (y + 22) & 0xff, count);
  }
  Oam_PutSprite(194, ((y + 10) & 0xff) | DIMMED(count), data_ov006_0236aa28,
                0xc000);
  count = (s8)OV6->unk_2c[2];
  if (count != 0) {
    Oam_PutSmallNumber(222, (y + 22) & 0xff, count);
  }
  Oam_PutSprite(214, ((y + 10) & 0xff) | DIMMED(count), data_ov006_0236aa18,
                0xd000);
  count = (s8)OV6->unk_2c[3];
  if (count != 0) {
    Oam_PutSmallNumber(242, (y + 22) & 0xff, count);
  }
  Oam_PutSprite(234, ((y + 10) & 0xff) | DIMMED(count), data_ov006_0236a9e8,
                0xe000);

  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
