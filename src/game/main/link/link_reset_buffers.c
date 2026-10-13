#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;
extern Unk_0229ca88 data_0229ca88;
extern Unk_0229cdfc data_0229cdfc[7];
extern Unk_0229cb4c data_0229cb4c[7];

void func_02080668(void);
void func_0208109c(void);

void Link_ResetBuffers(void) {
  int i;

  data_0229ca88.writeIndex = 0;
  data_0229ca88.readIndex = data_0229ca88.writeIndex;
  for (i = 0; i < 7; i++) {
    data_0229cdfc[i].writeIndex = 0;
    data_0229cdfc[i].readIndex = data_0229cdfc[i].writeIndex;
    data_0229cb4c[i].writeIndex = 0;
    data_0229cb4c[i].readIndex = data_0229cb4c[i].writeIndex;
    gLinkLastSequence[i] = 0x80;
  }
  for (i = 0; i < 8; i++) {
    data_0229ca48[i] = 0;
  }
  func_02080668();
  func_0208109c();
  data_0215fe8c->stopAcks = 0;
}
