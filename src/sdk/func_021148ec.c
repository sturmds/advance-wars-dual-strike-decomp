#include "types.h"
#include "sdk/pm.h"
#include "game/sound.h"

void PowerHook_AddFirst(PowerHookEntry** list, int fadeFrames);
extern PowerHookEntry* data_022ab8f0;

void func_021148ec(int fadeFrames) {
  PowerHook_AddFirst(&data_022ab8f0, fadeFrames);
}
