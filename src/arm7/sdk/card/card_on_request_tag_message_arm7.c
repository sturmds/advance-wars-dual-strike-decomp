#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/os_arm7.h"

/*
 * PXI tag 11 message from the other CPU. The first word is a request number;
 * GAMECARD_REQ_HANDSHAKE is followed by a second word, the command block address.
 * Request 1 is never queued. Once a request has all its words, it wakes the
 * card thread, or the thread waiting for the running request.
 */
void Card_OnRequestTagMessage_Arm7(u32 tag, u32 data, int err) {
  CARDiArm7Common* common;

  if (tag != FIFO_TAG_CARD_SERVICE || !err) {
    return;
  }
  common = &data_arm7_03809280;
  if (common->recvStep == 0) {
    common->command = data;
  }
  switch (common->command) {
    case GAMECARD_REQ_HANDSHAKE:
      switch (common->recvStep) {
        case 0:
          break;
        case 1:
          common->cmd = (CARDiCommandArg*)data;
          common->flag |= GAMECARD_FLAG_PEER_JOB_PENDING;
          break;
      }
      break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      common->flag |= GAMECARD_FLAG_PEER_JOB_PENDING;
      break;
  }
  if (!(common->flag & GAMECARD_FLAG_PEER_JOB_PENDING)) {
    common->recvStep++;
  } else {
    common->recvStep = 0;
    if (common->flag & CARD_STAT_BUSY) {
      Thread_MakeReady_Arm7(common->waiter);
    } else {
      Thread_MakeReady_Arm7(&common->thread);
    }
  }
}
