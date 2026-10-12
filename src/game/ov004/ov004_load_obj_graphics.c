#include "types.h"
#include "game/screen_other_map.h"

extern const char data_ov004_022bdd6c[];
extern const char data_ov004_022bde2c[];

void Ov004_CopyResource(const char* name, void* dest);
void Ov004_UnpackResource(const char* name, void* dest);
void Palette_Flush(void);

void Ov004_LoadObjGraphics(void) {
  Ov004_UnpackResource(data_ov004_022bde2c, data_027c0280->objVram);
  Ov004_CopyResource(data_ov004_022bdd6c, data_027c0280->paletteBuffer + 0x200);
  Palette_Flush();
}
