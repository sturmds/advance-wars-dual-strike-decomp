#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ScreenConfig_RunActionHook(void) {
  if (data_027c027c->unk_50[data_027c0284]->unk_48 == NULL) {
    return;
  }
  data_027c027c->unk_50[data_027c0284]->unk_48();
}
