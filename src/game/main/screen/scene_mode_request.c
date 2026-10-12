#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

/* Requests a change to mode; returns 0 when it is the current mode already. */
int SceneMode_Request(int mode) {
  Unk_021694c0* scene = data_021694c0;
  if (mode == scene->mode) {
    return 0;
  }
  scene->requestedMode = mode;
  return 1;
}
