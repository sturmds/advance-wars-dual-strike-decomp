#include "types.h"
#include "game/co_graphics.h"
#include "game/co_data.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
int Co_GetPictureVariant(int coId);

extern char data_02151e44[]; /* "/syogun" */

/* Co_LoadGraphics into a buffer: unpacks the picture of a CO (stored variant)
 * from /syogun to dest. */
void Co_LoadGraphicsToBuffer(int coId, void* dest) {
  FS_ChangeDir(data_02151e44);
  File_UnpackTo(
      dest,
      gCoGraphics[coId & CO_ID_MASK].pictures[Co_GetPictureVariant(coId)].name);
}
