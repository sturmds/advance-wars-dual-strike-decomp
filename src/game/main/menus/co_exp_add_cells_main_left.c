#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void CoExp_AddCellSprite(int x, int y);
void func_02032264(u32 x, u32 y, int matrix);
BOOL CoExp_IsSpecialMission(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Starts CO cell sprites on both screens, at the left of the main screen
 * (64, 152) and the right of the sub screen (192, 40). The second task is
 * added when CoExp_IsSpecialMission says so. */
void CoExp_AddCellsMainLeft(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  CoExp_AddCellSprite(64, 152);
  if (CoExp_IsSpecialMission()) {
    func_02032264(32, 120, 29);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  CoExp_AddCellSprite(192, 40);
  if (CoExp_IsSpecialMission()) {
    func_02032264(160, 8, 28);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
