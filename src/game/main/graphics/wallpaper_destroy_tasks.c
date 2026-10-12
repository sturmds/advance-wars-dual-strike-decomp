#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void Task_DestroyByScript(const void* script);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_0215937c[];
extern u8 gWallpaperTaskScript[];

/* Destroys the tasks started from gWallpaperTaskScript and data_0215937c (the
 * wallpaper task of Wallpaper_LoadBoth) on both screens. */
void Wallpaper_DestroyTasks(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Task_DestroyByScript(gWallpaperTaskScript);
  Task_DestroyByScript(data_0215937c);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Task_DestroyByScript(gWallpaperTaskScript);
  Task_DestroyByScript(data_0215937c);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
