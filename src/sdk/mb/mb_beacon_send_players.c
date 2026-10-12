#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayAdvertProgress data_022ace4c;
extern DlPlayRosterFrame
    data_022ace60; /* the DlPlayAdvertFrame buffer, in its player-list form */
extern u32 data_022ace44; /* 0: call data_022ace48 before sending, 1: after */
extern void (*data_022ace48)(u32 ggid);

void MIi_CpuClear16(u16 value, void* dst, u32 size);
void MIi_CpuCopy16(const void* src, void* dst, u32 size);
u16 MbBeacon_Checksum(const u16* data, int size);
void MbBeacon_EndGame(void);
int WM_SetGameInfo(void (*callback)(void* event), void* userGameInfo,
                   u16 length, u32 ggid, u16 tgid, u8 attribute);

/* Sends one beacon with the current game's player list, up to four players
 * not sent yet. Once every player went out, the beacon picks the next game. */
void MbBeacon_SendPlayers(u32 ggid, u16 tgid, u32 attribute) {
  volatile u16
      zero; /* needed to match: the fill value is read back from the stack */
  u32 count;
  int i;
  u16 pending;

  if (data_022ace4c.changeCount != data_022ace4c.currentGame->changeCount) {
    MbBeacon_EndGame();
  }
  data_022ace60.kind = DLPLAY_ADVERT_IS_ROSTER;
  data_022ace60.seqNo = data_022ace4c.currentGame->seqNo;
  data_022ace60.changeCount = data_022ace4c.changeCount;
  data_022ace60.ggid = data_022ace4c.currentGame->ggid;
  data_022ace60.fileNo = data_022ace4c.currentGame->fileNo;
  data_022ace60.sequence = data_022ace4c.sequence++;
  data_022ace60.playerCount = data_022ace4c.currentGame->playerCount;
  data_022ace60.playerFlags = data_022ace4c.currentGame->playerFlags;
  data_022ace60.unk_35c = data_022ace4c.currentGame->unk_35c;
  if (data_022ace44 == 0 && data_022ace48 != NULL) {
    data_022ace48(data_022ace4c.currentGame->ggid);
  }
  for (i = 0; i < 8; i++) {
    data_022ace60.unk_4a8[i] = data_022ace4c.currentGame->unk_4a8[i];
  }

  zero = 0;
  MIi_CpuClear16(zero, data_022ace60.users, sizeof(data_022ace60.users));
  count = 0;
  pending = data_022ace4c.currentGame->sentPlayerFlags ^
            data_022ace4c.currentGame->playerFlags;
  for (i = 0; i < DLPLAY_CHILD_SLOTS; i++) {
    if (pending & (2 << i)) { /* bit 0 is the parent */
      MIi_CpuCopy16(data_022ace4c.currentGame->users[i],
                    data_022ace60.users[count], DLPLAY_PLAYER_BADGE_BYTES);
      count++;
      data_022ace4c.currentGame->sentPlayerFlags |= 2 << i;
      if (count == DLPLAY_ROSTER_PER_FRAME) {
        break;
      }
    }
  }
  if (count < DLPLAY_ROSTER_PER_FRAME) {
    data_022ace60.users[count][0] &= ~0xf0; /* player number 0 ends the list */
  }

  data_022ace60.checksum = 0;
  data_022ace60.checksum =
      MbBeacon_Checksum(&data_022ace60.checksum, DLPLAY_ADVERT_SUMMED_BYTES);
  if (data_022ace4c.currentGame->sentPlayerFlags ==
      data_022ace4c.currentGame->playerFlags) {
    data_022ace4c.state = DLPLAY_ADVERT_STEP_PICK;
  }
  WM_SetGameInfo(0, &data_022ace60, sizeof(DlPlayRosterFrame), ggid, tgid,
                 attribute | WM_ATTR_FLAG_MB);
  if (data_022ace44 == 1 && data_022ace48 != NULL) {
    data_022ace48(data_022ace4c.currentGame->ggid);
  }
}
