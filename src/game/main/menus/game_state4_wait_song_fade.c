#include "types.h"
#include "game/Unk_02080440.h"
#include "game/song.h"

/* Handler for game state 4: goes to state 5 once the song has finished
 * fading out. */
void Game_State4_WaitSongFade(void) {
  if (Sound_IsSongFadingOut()) {
    return;
  }
  MAP_STATE->unk_18 = 5;
}
