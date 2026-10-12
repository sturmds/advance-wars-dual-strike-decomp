#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

/* 1 when mode is the scene's current mode. */
int SceneMode_IsCurrent(int mode) {
  return (mode == data_021694c0->mode) ? 1 : 0;
}
