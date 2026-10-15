#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Task_DestroyByScript(const void* script);

extern TaskScriptCmd gSpriteTaskScript[];

/* Destroys the sprite tasks of the first screen (data_027c0288.unk_00). */
void SpriteTask_DestroyAllFirstScreen(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Task_DestroyByScript(gSpriteTaskScript);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
