#include "types.h"
#include "game/sound.h"

void PowerHook_Unlink(void** handle, int fadeFrames);
extern void* data_022ab8f8;

void func_021148a4(int fadeFrames) {
  PowerHook_Unlink(&data_022ab8f8, fadeFrames);
}
