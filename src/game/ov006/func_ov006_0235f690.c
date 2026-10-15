#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int Player_CountUnits(int player);

void func_ov006_0235f690(void) {
  data_ov006_0236d640->unk_2c[0] = Player_CountUnits(1);
  data_ov006_0236d640->unk_2c[1] = Player_CountUnits(2);
  data_ov006_0236d640->unk_2c[2] = Player_CountUnits(3);
  data_ov006_0236d640->unk_2c[3] = Player_CountUnits(4);
}
