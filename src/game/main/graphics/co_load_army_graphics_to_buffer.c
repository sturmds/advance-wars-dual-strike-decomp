#include "types.h"
#include "game/co_graphics.h"
#include "game/co_data.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
int Co_GetArmyPictureVariant(int coId, int army);

extern char data_02151e44[]; /* "/syogun" */

/* Co_LoadArmyGraphics into a buffer: unpacks the picture of a CO in an
 * army's variant from /syogun to dest. */
void Co_LoadArmyGraphicsToBuffer(int coId, void* dest, int army) {
  FS_ChangeDir(data_02151e44);
  File_UnpackTo(dest, gCoGraphics[coId & CO_ID_MASK]
                          .pictures[Co_GetArmyPictureVariant(coId, army)]
                          .name);
}
