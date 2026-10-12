#include "types.h"
#include "game/event_script.h"
#include "game/song.h"

Task* Task_FindByScript(const void* script);
BOOL Campaign_IsHardMode(void);

/* Script command: waits (does not advance) while a song request task runs,
 * then fades the song out in hard mode only. */
BOOL EventScript_FadeOutSongHardMode(s16 index) {
  if (Task_FindByScript(gSongRequestScript) != NULL) {
    return FALSE;
  }
  if (Campaign_IsHardMode() == TRUE) {
    Sound_FadeOutSong();
  }
  data_022977f0[index].pc++;
  return FALSE;
}
