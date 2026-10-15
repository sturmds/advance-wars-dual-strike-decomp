#include "types.h"
#include "game/co_graphics.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "sdk/fs.h"

int Co_GetPictureVariant(int coId);
void File_UnpackTo(void* dest, const char* fileName);

extern char data_02151e44[]; /* "/syogun" */

/* Loads the picture of a CO from /syogun into the tiles starting at
 * firstTile. */
void Co_LoadGraphics(int coId, int firstTile) {
  int variant;

  FS_ChangeDir(data_02151e44);
  variant = Co_GetPictureVariant(coId);
  File_UnpackTo(data_027c0280->objVram + firstTile * 32,
                gCoGraphics[coId & CO_ID_MASK].pictures[variant].name);
}
