#include "types.h"
#include "game/palette_fade.h"

void Mem_Free(void* block);

/* Frees the saved colors if there are any. */
void PaletteFade_FreeColors(PaletteFadeTask* fade) {
  if (fade->colors == NULL) {
    return;
  }
  Mem_Free(fade->colors);
}
