#include "types.h"
#include "game/event_script.h"
#include "game/song.h"

/* Script command: fades the song out over unk_08 * 16 frames. */
BOOL EventScript_FadeOutSong(s16 index) {
  Sound_FadeOutSongOver(data_022977f0[index].pc->unk_08);
  data_022977f0[index].pc++;
  return FALSE;
}
