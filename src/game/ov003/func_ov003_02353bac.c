#include "types.h"

s8 FrameCallback_Remove(const void* descriptor);
extern u8 Ov003_UploadFxColorsAndTiles[];
extern u8 Ov003_UploadFxTiles[];

void func_ov003_02353bac(void) {
  FrameCallback_Remove(Ov003_UploadFxColorsAndTiles);
  FrameCallback_Remove(Ov003_UploadFxTiles);
}
