#include "types.h"

void Ov006_LoadTerrainIcons(void);
void func_ov006_02352400(void);

void func_ov006_023526c8(int arg) {
  if (arg == 1) {
    func_ov006_02352400();
  } else if (arg == 0) {
    Ov006_LoadTerrainIcons();
  }
}
