#include "types.h"
#include "game/sound_arc.h"

SoundArc* func_021066ec(SoundArc* arc) {
  SoundArc* previous = gCurrentSoundArc;
  gCurrentSoundArc = arc;
  return previous;
}
