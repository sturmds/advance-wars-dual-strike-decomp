#include "types.h"
#include "game/event_script.h"
#include "game/screen_other_map.h"
#include "game/task.h"

Task* Task_FindByScript(const void* script);

/* Event script command: waits until no task runs the script at +4 on either
 * screen. Returns FALSE (and stays on the command) while one does. */
BOOL EventScript_WaitTaskBothScreens(s16 index) {
  const void* script = EVENT_SCRIPT_DATA_COMMAND(index)->target;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  BOOL running;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  running = Task_FindByScript(script) != NULL;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  if (running) {
    return FALSE;
  }

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  running = Task_FindByScript(script) != NULL;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  if (running) {
    return FALSE;
  }

  data_022977f0[index].pc++;
  return TRUE;
}
