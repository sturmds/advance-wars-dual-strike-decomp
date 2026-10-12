#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

int SceneMode_Request(int value);
void EffectScene_SetMode(int value);

void func_020f20fc(void) {
  SceneMode_Request(7);
  EffectScene_SetMode(4);
  data_021694c0->unk_3c = 0;
}
