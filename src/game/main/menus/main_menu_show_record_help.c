#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Bg_ClearRect(u16* map, int x, int y, int width, u16 height);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Bg_SetRectPalette(u16* map, int x, int y, int width, u16 height,
                       u16 palette);
void Gfx_RequestLayerCopy(int layer);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_02167c88[];
/* Three fields of a table of 16-byte records; the table has its own
 * symbols per field, so each is an array of rows. */
extern u8 data_02290a62[][16]; /* 1: draws an extra mark at the end */
extern u8 data_02290a64[][16]; /* a number; 0xff: none */
extern u16 data_02290a66[][8]; /* a second number; 0: not shown */

/* Draws the help line of main menu entry kinds 31-33 on row 22 of the other
 * screen's layer 0: a fixed text, then the numbers of record index (or a
 * "none" text), each piece printed one after the other. x is the tile
 * column, tile the first tile each printed text may use. */
void MainMenu_ShowRecordHelp(int index) {
  int tile = 0x3c0;
  u8 x = 0;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Bg_ClearRect(data_027c0280->layer0Map, 0, 22, 32, 2);
  Text_PrintId(x, 22, data_027c0280->layer0Map, 0x8000002b, 0, tile);
  if (data_02290a64[index][0] == 0xff) {
    Text_PrintId(8, 22, data_027c0280->layer0Map, 0x8000002c, 0, 0x3d0);
    Text_PrintId(9, 22, data_027c0280->layer0Map, 0x8000002e, 0, 0x3d2);
    x += 13;
    tile += 26;
    if (data_02290a62[index][0] == 1) {
      Text_Print(x, 22, data_027c0280->layer0Map, data_02167c88, 0, tile);
      x++;
      Text_PrintId(x, 22, data_027c0280->layer0Map, 0x8000002f, 0, tile + 2);
      x += 3;
      tile += 8;
    }
  } else {
    Text_PrintId(8, 22, data_027c0280->layer0Map, 0x8000002c, 0, 0x3d0);
    Text_PrintId(9, 22, data_027c0280->layer0Map, 0x80000029, 0, 0x3d2);
    Number_Draw(15, 22, data_027c0280->layer0Map, data_02290a64[index][0], 0,
                0);
    x += 16;
    if (data_02290a66[index][0] != 0) {
      Text_Print(x, 22, data_027c0280->layer0Map, data_02167c88, 0, 0x3e0);
      x++;
      Text_PrintId(x, 22, data_027c0280->layer0Map, 0xc00000ad, 0, 0x3e2);
      Number_Draw(x + 4, 22, data_027c0280->layer0Map, data_02290a66[index][0],
                  0, 0);
      x += 5;
    }
    if (data_02290a62[index][0] == 1) {
      Text_Print(x, 22, data_027c0280->layer0Map, data_02167c88, 0, 0x3e6);
      x++;
      Text_PrintId(x, 22, data_027c0280->layer0Map, 0x8000002a, 0, 1000);
      x += 3;
    }
    tile += 46;
  }
  Text_PrintId(x, 22, data_027c0280->layer0Map, 0x8000002d, 0, tile);
  Bg_SetRectPalette(data_027c0280->layer0Map, 0, 22, 32, 2, 1);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
