#include "types.h"
#include "game/co_data.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void* Mem_Alloc(u32 size);
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
void DeferredCopyTask_Start(void* src, void* dest, u32 size);

extern u32 gLinkPlay;
extern char data_02151e44[]; /* "/syogun" */
extern const char* const data_02152b48[][7][3];

/* Co_LoadArmyLargePicture with the unpack going to a heap buffer, copied to dest by a
 * task (DeferredCopyTask_Start). The palette is loaded at once. */
void Co_QueueLargePictureCopy(int coMood, void* dest, int paletteSlot,
                              int army) {
  u8* buffer;
  int variant;

  if (gLinkPlay == 0 || (coMood & CO_CLONE)) {
    army = 0;
  }
  FS_ChangeDir(data_02151e44);
  buffer = Mem_Alloc(0x480);
  variant = Co_GetArmyPictureVariant(coMood, army);
  File_UnpackTo(
      buffer,
      data_02152b48[coMood & CO_ID_MASK][variant][(coMood & 0x300) >> 8]);
  DeferredCopyTask_Start(buffer, dest, 0x480);
  Co_LoadArmyPalette(coMood, paletteSlot, army);
}
