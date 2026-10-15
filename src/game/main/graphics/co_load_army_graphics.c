#include "types.h"
#include "game/co_graphics.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
int Co_GetArmyPictureVariant(int coId, int army);

extern u32 gLinkPlay;
extern char data_02151e44[]; /* "/syogun" */

/* Unpacks the picture of a CO from /syogun into sprite tile memory, starting
 * at tile `tile` (32 bytes each). Bit 7 of coId, or gLinkPlay being 0,
 * means army 0. */
void Co_LoadArmyGraphics(int coId, int tile, int army) {
  if (gLinkPlay == 0 || (coId & CO_CLONE)) {
    army = 0;
  }
  FS_ChangeDir(data_02151e44);
  File_UnpackTo(data_027c0280->objVram + tile * 32,
                gCoGraphics[coId & CO_ID_MASK]
                    .pictures[Co_GetArmyPictureVariant(coId, army)]
                    .name);
}
