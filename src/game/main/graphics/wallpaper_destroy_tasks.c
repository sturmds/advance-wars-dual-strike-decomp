#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void Task_DestroyByScript(const void* script);

extern TaskScriptCmd data_0215937c[];
extern TaskScriptCmd gWallpaperTaskScript[];

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
