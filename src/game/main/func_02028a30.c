#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void func_02028d50(void);
void Screen_InitLayers(const void* config);
void Screen_SetLayerDefaults(void);
extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern GfxLayer data_0215948c[];
extern GfxLayer data_021594dc[];

void func_02028a30(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_02028d50();
  Screen_InitLayers(data_0215948c);
  Screen_SetLayerDefaults();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_02028d50();
  Screen_InitLayers(data_021594dc);
  Screen_SetLayerDefaults();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
