#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayAdvertProgress data_022ace4c;
extern DlPlayAdvertFrame data_022ace60;

void MIi_CpuClear16(u16 value, void* dst, u32 size);
void MIi_CpuCopy16(const void* src, void* dst, u32 size);
u16 MbBeacon_Checksum(void* data, u32 size);
int WM_SetGameInfo(void (*callback)(void* event), void* userGameInfo,
                   u16 length, u32 ggid, u16 tgid, u8 attribute);

/* Sends the next piece of the current game as one beacon (WM_SetGameInfo),
 * zero-filling the payload after a short last piece. */
void MbBeacon_SendPiece(u32 ggid, u16 tgid, u32 attribute) {
  u8* gameEnd = (u8*)&data_022ace4c.currentGame->playerCount;
  volatile u16
      zero; /* needed to match: the fill value is read back from the stack */

  if (data_022ace4c.nextPiece + DLPLAY_ADVERT_CHUNK_BYTES <= gameEnd) {
    data_022ace60.size = DLPLAY_ADVERT_CHUNK_BYTES;
  } else {
    data_022ace60.size = gameEnd - data_022ace4c.nextPiece;
    zero = 0;
    MIi_CpuClear16(zero, &data_022ace60.data[data_022ace60.size],
                   DLPLAY_ADVERT_CHUNK_BYTES - data_022ace60.size);
  }
  MIi_CpuCopy16(data_022ace4c.nextPiece, data_022ace60.data,
                data_022ace60.size);

  data_022ace60.pieceIndex = data_022ace4c.pieceIndex;
  data_022ace60.pieceCount = data_022ace4c.pieceCount;
  data_022ace60.noIcon = data_022ace4c.currentGame->noIcon;
  data_022ace60.seqNo = data_022ace4c.currentGame->seqNo;
  data_022ace60.changeCount = data_022ace4c.changeCount;
  data_022ace60.ggid = data_022ace4c.currentGame->ggid;
  data_022ace60.fileNo = data_022ace4c.currentGame->fileNo;
  data_022ace60.sequence = data_022ace4c.sequence++;
  data_022ace60.checksum = 0;
  data_022ace60.checksum =
      MbBeacon_Checksum(&data_022ace60.checksum, DLPLAY_ADVERT_SUMMED_BYTES);

  data_022ace4c.pieceIndex++;
  if (data_022ace4c.pieceIndex < data_022ace4c.pieceCount) {
    data_022ace4c.nextPiece += DLPLAY_ADVERT_CHUNK_BYTES;
  } else {
    data_022ace4c.state = DLPLAY_ADVERT_STEP_DONE;
  }
  WM_SetGameInfo(0, &data_022ace60, sizeof(DlPlayAdvertFrame), ggid, tgid,
                 attribute | WM_ATTR_FLAG_MB);
}
