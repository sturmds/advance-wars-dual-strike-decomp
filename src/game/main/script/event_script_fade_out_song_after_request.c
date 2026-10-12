#include "types.h"
#include "game/event_script.h"
#include "game/song.h"

Task* Task_FindByScript(const void* script);

/* Script command: waits (does not advance) while a song request task runs,
 * then fades the song out. */
BOOL EventScript_FadeOutSongAfterRequest(s16 index) {
  if (Task_FindByScript(gSongRequestScript) != NULL) {
    return FALSE;
  }
  Sound_FadeOutSong();
  data_022977f0[index].pc++;
  return FALSE;
}
