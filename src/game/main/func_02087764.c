#include "types.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(int);

void func_02087764(int a0) {
  Sound_PlaySe(0x10002);
  data_02160ac0->unk_7e = a0;
}
