#include "types.h"
#include "game/co_graphics.h"
#include "game/Unk_02088d04.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
int Co_GetArmyPictureVariant(int coId, int army);

extern u32 gLinkPlay;
extern Unk_027c0280* data_027c0280;
extern char data_02151e44[]; /* "/syogun" */

/* Unpacks the picture of a CO from /syogun into sprite tile memory, starting
 * at tile `tile` (32 bytes each). Bit 7 of coId, or gLinkPlay being 0,
 * means army 0. */
void Co_LoadArmyGraphics(int coId, int tile, int army) {
  if (gLinkPlay == 0 || (coId & 0x80)) {
    army = 0;
  }
  FS_ChangeDir(data_02151e44);
  File_UnpackTo(data_027c0280->objVram + tile * 32,
                gCoGraphics[coId & 0x7f]
                    .pictures[Co_GetArmyPictureVariant(coId, army)]
                    .name);
}
