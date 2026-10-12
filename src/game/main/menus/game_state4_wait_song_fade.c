#include "types.h"
#include "game/Unk_02080440.h"
#include "game/song.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Handler for game state 4: goes to state 5 once the song has finished
 * fading out. */
void Game_State4_WaitSongFade(void) {
  if (Sound_IsSongFadingOut()) {
    return;
  }
  STATE->unk_18 = 5;
}
