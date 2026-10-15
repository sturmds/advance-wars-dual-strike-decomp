#include "types.h"
#include "game/Unk_02080440.h"

void func_0201e954(void);
void EventScript_StartForGameMode(void);

void func_020bd8c0(int unused, u32 value) {
  data_027c027c->unk_48[data_027c0284]->unk_14 = value;
  func_0201e954();
  EventScript_StartForGameMode();
}
