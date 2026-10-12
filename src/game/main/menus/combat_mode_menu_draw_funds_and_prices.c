#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160ac0.h"

void Gfx_RequestLayerCopy(int layer);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 a, u16 b);
void Bg_ClearRect(void* target, int x, int y, int w, int h);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* One priced entry; greyed out when 20 cells are in use or the funds are
 * below the cost. */
#define ENTRY(y, text, cost)                                              \
  if (data_02160ac0->cellCount != 20 && data_02160ac0->funds >= (cost)) { \
    grey = 0;                                                             \
  } else {                                                                \
    grey = 1;                                                             \
  }                                                                       \
  Text_PrintImmediate(3, (y), data_027c0280->layer0Map, (text), 0, grey); \
  Number_Draw(13, (y), data_027c0280->layer0Map, (cost), 0, grey)

/* Draws the Combat mode funds (data_02160ac0->funds) and four entries with
 * their costs (1000, 3000, 5000, 6000) on layer 0 of the screen in
 * data_027c0288.unk_00. */
void CombatModeMenu_DrawFundsAndPrices(void) {
  int grey;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Bg_ClearRect(data_027c0280->layer0Map, 1, 1, 13, 10);
  Text_PrintImmediate(1, 1, data_027c0280->layer0Map, 0x44000002, 0, 0);
  Number_Draw(13, 1, data_027c0280->layer0Map, data_02160ac0->funds, 0, 0);
  ENTRY(3, 0xc0000001, 1000);
  ENTRY(5, 0xc0000005, 3000);
  ENTRY(7, 0xc0000004, 5000);
  ENTRY(9, 0xc0000007, 6000);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
