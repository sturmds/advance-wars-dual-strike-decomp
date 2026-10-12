#include "types.h"
#include "game/screen_other_map.h"

void func_01ff9c28(void (*callback)(void));
void MainLoop_SetFrameCallback(void (*callback)(void));
void func_ov003_023517e0(void);
void Ov003_InitScreens(void);
void Ov003_InitState(void);
void func_ov003_02364854(void);
void MainLoop_Ov003VBlank(void);
void Ov003_DrawFrame(void);
void Screen_SetCurrentBoth(void* state);

/* Sets up the overlay on the first screen, then installs
 * MainLoop_Ov003VBlank and Ov003_DrawFrame as the two main loop callbacks. */
void Ov003_Init(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_01ff9c28(NULL);
  MainLoop_SetFrameCallback(NULL);
  func_ov003_023517e0();
  Ov003_InitScreens();
  Ov003_InitState();
  func_ov003_02364854();
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  func_01ff9c28(MainLoop_Ov003VBlank);
  MainLoop_SetFrameCallback(Ov003_DrawFrame);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
