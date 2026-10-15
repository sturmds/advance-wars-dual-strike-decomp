#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440_50 data_02169f48[];

/* Makes record index of the screen config table the current screen's config. */
void ScreenConfig_Select(int index) {
  data_027c027c->unk_50[data_027c0284] = &data_02169f48[index];
}
