#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_PackArmySlots(int a, int b);
int Ov006_IsMapPlayable(void);
void DesignMap_SaveToSlot(int id, u8* buf, int arg);
extern Unk_ov006_0235f690* data_ov006_0236d640;

void func_ov006_02357d40(void) {
  int saved;
  Unk_ov006_0235f690* state;

  saved = Ov006_PackArmySlots(0, 0);
  state = data_ov006_0236d640;
  DesignMap_SaveToSlot(state->chosenSlot, state->unk_d8, Ov006_IsMapPlayable());
  Ov006_PackArmySlots(1, saved);
  if (data_ov006_0236d640->unk_00 & 0x100) {
    data_ov006_0236d640->unk_00 &= ~0x100;
    data_ov006_0236d640->unk_d8[0] = 0;
  }
  data_ov006_0236d640->unk_00 &= ~0x1000;
}
