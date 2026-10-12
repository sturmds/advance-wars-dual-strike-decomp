#include "types.h"
#include "game/Unk_02080440.h"

BOOL Campaign_IsMap248FirstState(void);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern char gTileCodeFileMap248First[]; /* "002" */
extern char gTileCodeFileDefault[];     /* "003" */
extern char gBmapDir[];                 /* "/bmap" */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Loads the tile code to terrain table (tileCodeTerrain) from /bmap: file
 * 002 in the first state of map 248, file 003 otherwise. */
void TileCodeTable_Load(void) {
  u8* table = STATE->tileCodeTerrain;
  const char* fileName;

  if (Campaign_IsMap248FirstState()) {
    fileName = gTileCodeFileMap248First;
  } else {
    fileName = gTileCodeFileDefault;
  }
  FS_ChangeDir(gBmapDir);
  File_UnpackTo(table, fileName);
}
