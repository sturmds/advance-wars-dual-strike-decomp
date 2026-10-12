#include "types.h"

s8 CallbackSlots_Add(const void* descriptor);
extern u8 Ov003_UploadFxColorsAndTiles[];
extern u8 Ov003_UploadFxTiles[];

void func_ov003_02353bd8(void) {
  CallbackSlots_Add(Ov003_UploadFxColorsAndTiles);
  CallbackSlots_Add(Ov003_UploadFxTiles);
}
