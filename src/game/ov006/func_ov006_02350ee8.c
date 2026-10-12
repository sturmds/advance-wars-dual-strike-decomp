#include "types.h"

void Sound_PlaySe(u32 soundId);
void Ov006_PostRequest(int request);

void func_ov006_02350ee8(void) {
  Sound_PlaySe(0x10009);
  Ov006_PostRequest(1);
}
