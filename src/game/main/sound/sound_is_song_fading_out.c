#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/song.h"

void Screen_SetCurrent(Unk_02080440* state);
void Screen_SetCurrentGfx(Unk_027c0280* state);
void* Task_FindByScript(
    const void*
        script); /* task of the current screen running script, or NULL */

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* TRUE while a fade-out task runs on either screen (data_027c0288 holds
 * one state per screen; tasks live in the state). */
BOOL Sound_IsSongFadingOut(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  BOOL found;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  found = Task_FindByScript(gSongFadeOutScript) != NULL;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  if (found) {
    return TRUE;
  }

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  found = Task_FindByScript(gSongFadeOutScript) != NULL;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  return found;
}
