#include "types.h"
#include "game/Unk_0201d320.h"

extern u8 gEventSkipState;
extern Unk_0201d320 data_022977f4[];
void Sound_PlaySe(int songId);

s16 func_0201b358(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];
  BOOL done;
  int songId = script->unk_00->unk_0c;

  if (gEventSkipState == 1) {
    done = 1;
  } else {
    Sound_PlaySe(songId);
    done = 0;
  }
  script->unk_00++;
  return done;
}
