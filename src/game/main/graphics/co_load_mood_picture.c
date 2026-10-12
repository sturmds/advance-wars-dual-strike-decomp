#include "types.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
int Co_GetArmyPictureVariant(
    int coId, int army); /* picture variant to show for this CO and army */
void Co_LoadArmyPalette(int coId, int paletteSlot,
                        int army); /* loads the palette that goes with it */

extern u32 gLinkPlay;
extern char data_02151e44[]; /* "/syogun" */
/* gCoGraphics + 0x28: per CO, 3 file names (one per mood) for each picture variant. */
extern const char* const data_02152b60[][7][3];

/* Unpacks a CO picture from /syogun to dest and loads its palette into
 * paletteSlot. Bits 0-6 of coMood are the CO id, bits 8-9 the mood (which of
 * 3 pictures). The army picks the palette variant; it is ignored (0) unless
 * gLinkPlay is set, and when bit 7 is set. */
void Co_LoadMoodPicture(int coMood, void* dest, int paletteSlot, int army) {
  int variant;

  if (gLinkPlay == 0 || (coMood & 0x80)) {
    army = 0;
  }
  FS_ChangeDir(data_02151e44);
  variant = Co_GetArmyPictureVariant(coMood, army);
  File_UnpackTo(dest,
                data_02152b60[coMood & 0x7f][variant][(coMood & 0x300) >> 8]);
  Co_LoadArmyPalette(coMood, paletteSlot, army);
}
