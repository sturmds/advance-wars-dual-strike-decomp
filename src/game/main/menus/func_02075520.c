#include "types.h"
#include "game/Unk_02080440.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Font_LoadDigits(int baseTile);

void func_02075520(void) {
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  Font_LoadDigits(0);
  *data_027c027c->unk_40 = 1;
}
