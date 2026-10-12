#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

void Ov003_InitRows(void);
void Ov003_LoadScreenGraphics(void);
void Ov003_LoadMap(void);
void Ov003_ResetView(void);
void func_ov003_02353bd8(void);
void Ov003_InitUnitsAndCamera(void);
void Ov003_ClearItems(void);
u32 MainLoop_GetFrameCount(void);
void Ov003_RandSeed(u32 seed);
void Keypad_Init(void);

/* Seeds the random generator with the frame count, or with a fixed seed when
 * data_02160ac0->unk_7c is set (same flag func_ov003_023622b8 reads). */
void Ov003_InitState(void) {
  Ov003_InitRows();
  Ov003_LoadScreenGraphics();
  Ov003_LoadMap();
  Ov003_ResetView();
  func_ov003_02353bd8();
  Ov003_InitUnitsAndCamera();
  Ov003_ClearItems();
  if (data_02160ac0->unk_7c == 0) {
    Ov003_RandSeed(MainLoop_GetFrameCount());
  } else {
    Ov003_RandSeed(0x31527396);
  }
  Keypad_Init();
  data_ov003_02364928->unk_1df_1 = 0;
}
