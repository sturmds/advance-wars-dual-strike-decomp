#include "types.h"
#include "sdk/pm.h"
#include "game/sound.h"

void PowerHook_AddLast(PowerHookEntry** list, int fadeFrames);
extern PowerHookEntry* data_022ab8f8;

void func_021148d4(int fadeFrames) {
  PowerHook_AddLast(&data_022ab8f8, fadeFrames);
}
