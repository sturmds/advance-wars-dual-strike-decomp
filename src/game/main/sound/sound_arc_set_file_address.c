#include "types.h"
#include "game/sound_arc.h"

SoundArc* gCurrentSoundArc;

void SoundArc_SetFileAddress(int a0, int a1) {
  gCurrentSoundArc->files->entries[a0].loaded = (void*)a1;
}
