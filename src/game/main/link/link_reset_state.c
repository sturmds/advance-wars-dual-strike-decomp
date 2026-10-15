#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c* data_0215fe8c;

void Link_ResetBuffers(void);
void func_0207a7a4(int mode, int playerCount); /* playerCount: a guess */
void Wireless_ResetState(void);

/* Resets the wireless and link state when link mode starts. The player
 * limit given to the wireless code depends on the link mode in unk_8b:
 * 8 in mode 3, 2 in modes 1 and 2, else 4. */
void Link_ResetState(void) {
  Wireless_ResetState();
  data_0215fe8c->unk_0a = MAP_STATE->unk_8b;
  switch (MAP_STATE->unk_8b) {
    case 3:
      func_0207a7a4(MAP_STATE->unk_8b, 8);
      break;
    case 2:
      func_0207a7a4(MAP_STATE->unk_8b, 2);
      break;
    case 0:
    case 4: /* needed to match: makes the jump table cover 0 to 4 */
    default:
      func_0207a7a4(MAP_STATE->unk_8b, 4);
      break;
    case 1:
      func_0207a7a4(MAP_STATE->unk_8b, 2);
      break;
  }
  data_0215fe8c->linkState = 0;
  data_0215fe8c->replyOk = 0;
  data_0215fe8c->error = 0;
  data_0215fe8c->linkActive = 0;
  data_0215fe8c->unk_0e = 0;
  data_0215fe8c->unk_0c = 0;
  data_0215fe8c->packetSize = 23;
  data_0215fe8c->nameSendAid = 0;
  data_0215fe8c->stopAcks = 0;
  data_0215fe8c->replyTimer = 0;
  Link_ResetBuffers();
}
