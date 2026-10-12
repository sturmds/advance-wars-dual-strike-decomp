#include "types.h"

void func_0210a948(void);
int File_UnpackTo(int, void*);
extern u8 data_02160aa0[];

/* Unpacks the 16x16 font that LargeFont_DrawName reads into LCDC VRAM bank A. */
int LargeFont_Load(void) {
  func_0210a948();
  return File_UnpackTo(0x6800000, data_02160aa0);
}
