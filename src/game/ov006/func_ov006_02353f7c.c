#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_0235402c(void);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void PopupMenu_OpenWithSound(const void* items, int x, int y, int arg,
                             int arg2);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236b15c[];

void func_ov006_02353f7c(void) {
  if (data_ov006_0236d640->unk_00 & 0x8000) {
    data_ov006_0236d640->unk_00 &= ~0x8000;
    data_ov006_0236d640->screenState = 0;
    func_ov006_0235402c();
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(1);
    func_020c4018();
    PopupMenu_OpenWithSound(data_ov006_0236b15c, 2, 2, 0, 0);
  }
  switch (data_ov006_0236d640->screenState) {
    case 0:
      data_ov006_0236d640->screenState = 10;
      break;
    case 10:
      break;
    case 20:
      break;
  }
}
