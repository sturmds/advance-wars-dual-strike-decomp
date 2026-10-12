#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Task_DestroyByScript(const void* script);
void func_020f3114(void);
void func_ov000_022b0054(void);
extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov000_023500f8[];

/* Destroys the second-screen task (script data_ov000_023500f8), then the
 * first-screen one (func_ov000_022b0054), then calls func_020f3114 on the
 * second screen. */
void SecondScreenTasks_Destroy(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Task_DestroyByScript(data_ov000_023500f8);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  func_ov000_022b0054();
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_020f3114();
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
