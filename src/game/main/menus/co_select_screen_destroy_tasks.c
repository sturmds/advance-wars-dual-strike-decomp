#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_button.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void PaletteCycle_Stop(void);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
int Task_DestroyByScript(const void* script);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const u8 data_0215a980[];

/* Removes the tasks and buttons of the CO select screen. The first
 * call runs on the screen of data_027c0288.unk_04. */
void CoSelectScreen_DestroyTasks(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  PaletteCycle_Stop();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Task_DestroyByScript(data_0215a980);
  Wallpaper_DestroyTasks();
  Screen1_DestroyTextTasks();
  MenuButtons_RemoveAll();
}
