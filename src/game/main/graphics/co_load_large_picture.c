#include "types.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
int Co_GetPictureVariant(int coId);
void Co_LoadPalette(int coId, int paletteSlot);

extern char data_02151e44[]; /* "/syogun" */
extern const char* const data_02152b48[][7][3];

/* Like Co_LoadMoodPicture, but for the 36-tile pictures, the stored variant
 * and no army: unpacks the picture to dest and loads its palette into
 * paletteSlot. */
void Co_LoadLargePicture(int coMood, void* dest, int paletteSlot) {
  FS_ChangeDir(data_02151e44);
  File_UnpackTo(dest, data_02152b48[coMood & 0x7f][Co_GetPictureVariant(coMood)]
                                   [(coMood & 0x300) >> 8]);
  Co_LoadPalette(coMood, paletteSlot);
}
