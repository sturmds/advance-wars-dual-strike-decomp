#include "types.h"
#include "game/Unk_02003f44.h"

extern u16 gScreenShakeY;
extern u16 gScreenShakeX;

void ScreenShake_Init(Unk_02003f44* seq) {
  gScreenShakeX = 0;
  gScreenShakeY = 0;
  seq->unk_54 = 0;
  seq->unk_58 = 0;
}
