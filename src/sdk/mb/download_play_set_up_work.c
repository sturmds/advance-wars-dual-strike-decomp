#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern u32 data_0216d9a0[];

/* points to L"multiboot" */
static const u16* data_0216d99c = (u16*)&data_0216d9a0;
u32 data_0216d9a0[5] = {
#include "data/main/data_0216d9a0.inc.c"
};
u16 gIsdFormatActions[8][9] = {
#include "data/main/gIsdFormatActions.inc.c"
};
u16 gIsdFormatCharClass[256] = {
#include "data/main/gIsdFormatCharClass.inc.c"
};

void MIi_CpuClear16(u16 value, void* dst, u32 size);
void MIi_CpuClear32(u32 value, void* dst, u32 size);
void Mem_CopyBytes(const void* src, void* dst, u32 size);
u32 MbParent_GetBeaconJitter(void);

#define DLPLAY_PLAYER_NAME_CHARS 10

/* Sets up Download Play in the caller's buffer: the parent work and then the
 * comm work, each 32-byte aligned, cleared and given defaults (256-byte
 * parent packets, 8-byte child packets, up to 15 children). Returns
 * DLPLAY_RESULT_WRONG_PHASE when MB already runs. */
int DownloadPlay_SetUpWork(void* work, const DlPlayPlayerBadge* user, u32 ggid,
                           u32 tgid, u32 dma) {
  DlPlayStationArea* parent;
  DlPlayHostLinkArea* comm;
  int enabled;
  int i;
  u16* dst;
  volatile u32 zero32;
  volatile u16 zero16;

  if (data_022aced4 != NULL && data_022aced4->unk_1316) {
    return DLPLAY_RESULT_WRONG_PHASE;
  }
  parent = (DlPlayStationArea*)(((u32)work + 31) & ~31);
  comm = (DlPlayHostLinkArea*)(((u32)parent + sizeof(DlPlayStationArea) + 31) &
                               ~31);

  enabled = OS_DisableInterrupts();
  data_022aced0 = dma; /* needed to match: stored before the work pointers */
  data_022aced8 = parent;
  data_022aced4 = comm;
  zero32 = 0;
  MIi_CpuClear32(zero32, parent, sizeof(DlPlayStationArea));
  zero16 = 0;
  MIi_CpuClear16(zero16, comm, 0x1340); /* up to childUsers */

  dst = parent->userName;
  for (i = 0; i < user->nameLength; i++) {
    *dst++ = user->name[i];
  }
  dst = parent->unk_538;
  for (i = 0; i < 16; i++) {
    if (*data_0216d99c == 0) {
      break;
    }
    *dst++ = *data_0216d99c++;
  }
  Mem_CopyBytes(user, &comm->user, sizeof(DlPlayPlayerBadge));
  if (user->nameLength < DLPLAY_PLAYER_NAME_CHARS) {
    comm->user.name[user->nameLength] = 0;
  }

  parent->sendSize = 0x100;
  parent->unk_502 = 8;
  parent->mpSendSize = 0;
  parent->mpRecvSize = 0;
  parent->unk_52c = 1;
  parent->mpRecvBuffer = comm->unk_400;
  parent->unk_0e = 1;
  parent->unk_12 = 0;
  parent->unk_16 = 1;
  parent->unk_14 = 0;
  parent->ggid = ggid;
  parent->unk_0c = tgid;
  parent->beaconPeriod = MbParent_GetBeaconJitter() + 200;
  parent->maxChildren = DLPLAY_CHILD_SLOTS;
  parent->sendQueued = 0;
  parent->started = 0;
  comm->unk_1316 = 1;
  comm->mpStarting = 0;
  OS_RestoreInterrupts(enabled);
  return DLPLAY_RESULT_OK;
}
