#include "types.h"
#include "game/sound_arc.h"

int SoundArc_GetFileAddress(int a0) {
  SoundArcFileTable* files = gCurrentSoundArc->files;
  if ((u32)a0 < files->count) {
  }
  return ((u32)a0 >= files->count) ? 0 : (int)files->entries[a0].loaded;
}
