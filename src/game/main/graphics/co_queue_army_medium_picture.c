#include "types.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 ignored, void* dst, u32 size);
void DeferredCopyTask_Start(void* src, void* dest, u32 size);
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
extern u32 gLinkPlay;
extern char data_02151e44[];
extern const char* const data_02152b80[][21];

/* Loads the 18-tile (0x240 bytes) picture of a CO in an army's colours. The
 * file is unpacked to a heap buffer that a task copies to dest
 * (DeferredCopyTask_Start); the palette is loaded at once. CO 0 clears dest.
 * Bit 7 of coId, or gLinkPlay being 0, means army 0. */
void Co_QueueArmyMediumPicture(int coId, void* dest, int paletteSlot,
                               int army) {
  void* buffer;

  if (gLinkPlay == 0 || (coId & 0x80)) {
    army = 0;
  }
  if ((coId & 0x7f) == 0) {
    Mem_Zero32(0, dest, 0x240);
    return;
  }
  FS_ChangeDir(data_02151e44);
  buffer = Mem_Alloc(0x240);
  coId &= 0x7f;
  File_UnpackTo(buffer,
                data_02152b80[coId][Co_GetArmyPictureVariant(coId, army)]);
  DeferredCopyTask_Start(buffer, dest, 0x240);
  Co_LoadArmyPalette(coId, paletteSlot, army);
}
