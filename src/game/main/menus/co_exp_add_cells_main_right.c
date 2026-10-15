#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void CoExp_AddCellSprite(int x, int y);
void func_02032264(u32 x, u32 y, int matrix);
BOOL CoExp_IsSpecialMission(void);

/* CoExp_AddCellsMainLeft with the two screens swapped. */
void CoExp_AddCellsMainRight(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  CoExp_AddCellSprite(192, 40);
  if (CoExp_IsSpecialMission()) {
    func_02032264(160, 8, 28);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  CoExp_AddCellSprite(64, 152);
  if (CoExp_IsSpecialMission()) {
    func_02032264(32, 120, 29);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
