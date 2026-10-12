#include "types.h"

void func_0201e954(void);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void func_ov006_02354778(void);

void func_ov006_02354414(void) {
  func_0201e954();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  func_ov006_02354778();
}
