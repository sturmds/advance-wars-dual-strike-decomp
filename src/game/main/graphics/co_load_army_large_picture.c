#include "types.h"
#include "game/co_data.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
extern u32 gLinkPlay;
extern char data_02151e44[];
extern const char* const data_02152b48[][7][3];

/* Co_LoadLargePicture in the picture variant of army; army 0 outside link play
 * or for a clone (CO_CLONE). */
void Co_LoadArmyLargePicture(int coMood, void* dest, int paletteSlot,
                             int army) {
  int variant;

  if (gLinkPlay == 0 || (coMood & CO_CLONE)) {
    army = 0;
  }
  FS_ChangeDir(data_02151e44);
  variant = Co_GetArmyPictureVariant(coMood, army);
  File_UnpackTo(
      dest, data_02152b48[coMood & CO_ID_MASK][variant][(coMood & 0x300) >> 8]);
  Co_LoadArmyPalette(coMood, paletteSlot, army);
}
