#include "types.h"
#include "game/co_data.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Mem_Zero32(u32 ignored, void* dst, u32 size);
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
extern u32 gLinkPlay;
extern char data_02151e44[];
extern const char* const data_02152b80[][21];

/* Co_QueueArmyMediumPicture without the heap buffer: the 18-tile picture is
 * unpacked straight to dest. CO 0 clears dest. */
void Co_LoadArmyMediumPicture(int coId, void* dest, int paletteSlot, int army) {
  if (gLinkPlay == 0 || (coId & CO_CLONE)) {
    army = 0;
  }
  if ((coId & CO_ID_MASK) == 0) {
    Mem_Zero32(0, dest, 0x240);
    return;
  }
  FS_ChangeDir(data_02151e44);
  coId &= 0x7f;
  File_UnpackTo(dest,
                data_02152b80[coId][Co_GetArmyPictureVariant(coId, army)]);
  Co_LoadArmyPalette(coId, paletteSlot, army);
}
