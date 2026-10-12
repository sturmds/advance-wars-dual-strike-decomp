#include "sdk/snd.h"

void SndArm7_SetPlayerVariable(int playerNo, int varNo, s16 value) {
  data_arm7_0380808c->player[playerNo].variable[varNo] = value;
}
