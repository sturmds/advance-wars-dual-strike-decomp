#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea* data_022aced4;
extern u8 gMbLastFileInfoSent;

void MI_CpuFill8(void* dst, u8 value, u32 size);
void Mem_CopyBytes(const void* src, void* dst, u32 size);
u8* MbComm_WriteParentMessage(DlPlayHostMsgHead* header, u8* buffer);
int func_0211b458(u32 size, u16 bitmap, u8* buffer);

/* Sends the download info of one file to the accepted children that picked
 * it. Files take turns: the search starts after the file sent last time
 * (gMbLastFileInfoSent). */
int MbParent_SendFileInfo(void) {
  DlPlayHostMsgHead header;
  u8 childCounts[DLPLAY_OFFER_SLOTS];
  s8 fileId = -1;
  u16 bitmap = 0;
  u16 aid;
  u8 file;
  u8 i;
  u8* data;

  MI_CpuFill8(childCounts, 0, sizeof(childCounts));
  for (aid = 1; aid <= DLPLAY_CHILD_SLOTS; aid++) {
    if (data_022aced4->commState[aid - 1] == DLPLAY_CHILD_ADMITTED) {
      childCounts[data_022aced4->childFiles[aid - 1]]++;
    }
  }

  file = gMbLastFileInfoSent;
  for (i = 0; i < DLPLAY_OFFER_SLOTS; i++) {
    file = (file + 1) % DLPLAY_OFFER_SLOTS;
    if (data_022aced4->files[file].active && childCounts[file] != 0) {
      fileId = file;
      break;
    }
  }
  if (fileId == -1) {
    return DLPLAY_NOTHING_TRANSMITTED;
  }
  gMbLastFileInfoSent = fileId;

  for (aid = 1; aid <= DLPLAY_CHILD_SLOTS; aid++) {
    if (data_022aced4->commState[aid - 1] == DLPLAY_CHILD_ADMITTED &&
        data_022aced4->childFiles[aid - 1] == fileId) {
      bitmap |= 1 << aid;
    }
  }

  header.type = DLPLAY_TO_CHILD_LAYOUT;
  header.fileId = fileId;
  data = MbComm_WriteParentMessage(&header, data_022aced4->sendBuffer);
  if (data != NULL) {
    Mem_CopyBytes(data_022aced4->files[fileId].dlFileInfo, data,
                  DLPLAY_LAYOUT_SENT_BYTES);
  }
  return func_0211b458(DLPLAY_HOST_MSG_HEAD_BYTES + DLPLAY_LAYOUT_SENT_BYTES,
                       bitmap, data_022aced4->sendBuffer);
}
