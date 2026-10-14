#include "types.h"
#include "sdk/pm.h"
#include "game/sound.h"

void PowerHook_Unlink(PowerHookEntry** list, int fadeFrames);
extern PowerHookEntry* data_022ab8f8;

void func_021148a4(int fadeFrames) {
  PowerHook_Unlink(&data_022ab8f8, fadeFrames);
}
