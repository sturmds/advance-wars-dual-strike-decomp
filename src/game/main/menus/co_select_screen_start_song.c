#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Sound_StartSong(int song);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Starts song 33 in Survival, or when the state has musicReady set. */
void CoSelectScreen_StartSong(void) {
  if (STATE->gameMode == GAME_MODE_SURVIVAL || STATE->musicReady != 0) {
    Sound_StartSong(33);
  }
}
