#include "types.h"
#include "game/screen_shake.h"
#include "game/Unk_02003f44.h"

void ScreenShake_Init(Unk_02003f44* seq) {
  gScreenShakeX = 0;
  gScreenShakeY = 0;
  seq->unk_54 = 0;
  seq->unk_58 = 0;
}
