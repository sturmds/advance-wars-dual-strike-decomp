#include "types.h"
#include "game/sound_arc.h"

int func_021063d0(int a0) {
  SoundArcFileTable* files = gCurrentSoundArc->files;
  if ((u32)a0 < files->count) {
  }
  return ((u32)a0 >= files->count) ? 0 : files->entries[a0].size;
}
