#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiTurn_ClearCaptureMarks(void) {
  Unk_02080440_6088* entry;
  int i;

  entry = data_027c027c->unk_48[data_027c0284]->unk_6088;
  for (i = 0; i < MAP_SITE_COUNT; i++) {
    entry->claimCount = 0;
    entry->unk_04 = 0;
    entry->unk_05 = 0;
    entry++;
  }
}
