#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/player.h"

void Gfx_RequestLayerCopy(int layer);
void Screen_SetCurrentGfx(void* state);
void Bg_ClearRect(void* target, int x, int y, int w, int h);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_022a7add[][40]; /* data_022a7ab8[].unitCount */

#define GAME (data_027c027c->unk_48[data_027c0284])

/* On the sub screen: clears tile rows 5 to 16 of layer 0, then prints
 * three labelled numbers for one player slot: units left at the end
 * (text 0x7a0001c3), Player.unk_04 (0x7a0001c4; money spent in doc 890)
 * and damage taken in funds (0x7a0001c5). */
void ResultStats_DrawPlayerTotals(int player) {
  Unk_027c0280* saved0280 = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Bg_ClearRect(data_027c0280->layer0Map, 1, 5, 31, 12);
  Text_PrintImmediate(5, 7, data_027c0280->layer0Map, 0x7a0001c3, 0, 0);
  Number_Draw(26, 7, data_027c0280->layer0Map, data_022a7add[player][0], 0, 0);
  Text_PrintImmediate(5, 9, data_027c0280->layer0Map, 0x7a0001c4, 0, 0);
  Number_Draw(26, 9, data_027c0280->layer0Map, GAME->players[player].unk_04, 0,
              0);
  Text_PrintImmediate(5, 11, data_027c0280->layer0Map, 0x7a0001c5, 0, 0);
  Number_Draw(26, 11, data_027c0280->layer0Map,
              GAME->players[player].damageTaken, 0, 0);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrentGfx(saved0280);
}
