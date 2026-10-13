#include "types.h"
#include "game/second_screen.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/unit_stats.h"
#include "game/text_print.h"

void Font_SetDigitEndTile(int tile);
const void* Text_Get(u32 textId);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const void* text, u16 a,
                               u16 b);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 a, u16 b);
void Bg_PutTile(int layer, int x, int y, u16 entry);
int IncreaseFunds(int playerIndex, int terrain);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
/* Fields 0x10 and 0x14 of Ov000_gTerrainStats[terrain - 1] (rows of 6 words);
 * the symbols sit at the folded addresses. */
extern u32 data_ov000_022f4cc4[][6];
extern u32 data_ov000_022f4cc8[][6];
extern int data_ov000_023500b4[3]; /* unit types checked for repairs */
extern u32
    data_ov000_023500a8[3]; /* first map entry of each type's 3x2 picture */

/* Second screen: draws the text part of the terrain panel at pixel x. The terrain name
 * (TerrainStats.unk_10) goes centered on row 1, text 0xc00000a6 on row 3. Text 0xc00000af
 * goes on row 3 when data_ov000_02350548 is 0, and on row 5 when the terrain gives the
 * player in data_ov000_0235054c no funds (IncreaseFunds returns 0). For the first of the
 * three unit types that this terrain repairs, it draws a repair icon at row 7 and that
 * type's 3x2 picture beside it (that write adds x to a map index, unlike the rest).
 * TerrainStats.unk_14 goes on row 17. Which text ids say what is a guess. */
void SecondScreen_DrawTerrainDetails(int x, int terrain) {
  int tile;
  u16* map;
  int col;
  const void* text;
  int i;

  Font_SetDigitEndTile(98);
  text = Text_Get(data_ov000_022f4cc4[terrain][0]);
  Text_PrintStringImmediate((x + 84 - Text_GetPrintWidth(text) / 2) / 8, 1,
                            data_027c0280->layer0Map, text, 0, 0);
  Font_SetDigitEndTile(142);
  Text_PrintImmediate((x + 56) / 8, 3, data_027c0280->layer0Map, 0xc00000a6, 0,
                      0);
  if (data_ov000_02350548 == 0) {
    Text_PrintImmediate((x + 80) / 8, 3, data_027c0280->layer0Map, 0xc00000af,
                        0, 0);
  }
  Icon_LoadTo(223, data_027c0280->objVram, 232);
  if (IncreaseFunds(data_ov000_0235054c, terrain) == 0) {
    Text_PrintImmediate((x + 80) / 8, 5, data_027c0280->layer0Map, 0xc00000af,
                        0, 0);
  }
  for (i = 0; i < 3; i++) {
    if (Ov000_gUnitStats[data_ov000_023500b4[i]].repairTerrain[terrain] != 0) {
      Icon_LoadTo(224, data_027c0280->unk_e4, 158);
      Icon_LoadPalette(224, 13);
      Bg_PutTile(0, (x + 56) / 8, 7, 0xd09e);
      col = (x + 56) / 8 + 1;
      Bg_PutTile(0, col, 7, 0xd09f);
      Bg_PutTile(0, (x + 56) / 8, 8, 0xd0a0);
      Bg_PutTile(0, col, 8, 0xd0a1);
      col = (x + 56) / 8 + 2;
      Bg_PutTile(0, col, 7, 0xd0a2);
      Bg_PutTile(0, col, 8, 0xd0a3);
      map = data_027c0280->layer0Map;
      tile = data_ov000_023500a8[i];
      map += x;
      map[234] = tile;
      map[235] = tile + 1;
      map[266] = tile + 2;
      map[267] = tile + 3;
      map[236] = tile + 4;
      map[268] = tile + 5;
      break;
    }
  }
  Text_PrintId((x + 8) / 8, 17, data_027c0280->layer0Map,
               data_ov000_022f4cc8[terrain][0], 0, 164);
  Gfx_RequestLayerCopy(0);
}
