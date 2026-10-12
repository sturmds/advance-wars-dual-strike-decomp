#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void PopupMenu_PopState(void);
void Sound_PlaySe(int se);
void func_ov006_0235402c(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;

void func_ov006_02354778(void) {
  PopupMenu_PopState();
  Sound_PlaySe(0x10009);
  data_ov006_0236d640->request = 3;
  func_ov006_0235402c();
}
