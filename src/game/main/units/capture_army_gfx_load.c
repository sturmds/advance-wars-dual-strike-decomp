#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;
extern char* data_02168d30[]; /* file name per army style */
extern char data_02169180[];  /* "/bmap" */
extern char data_021691a8[];  /* "0b9" */

u8 Player_GetArmyStyle(int playerIndex);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          u16 paletteOffset, int size);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Loads the capture graphics of the owner's army from /bmap (file chosen by
 * army style) to sprite tile 532, and the owner's army colours from 0b9 to
 * sprite palette `palette`. gfxTile is not used: the tile is fixed. */
void CaptureArmyGfx_Load(int owner, int gfxTile, int palette) {
  int style = Player_GetArmyStyle(owner);
  int colour = STATE->players[owner].armyColour;

  FS_ChangeDir(data_02169180);
  File_UnpackTo(data_027c0280->objVram + 532 * 32, data_02168d30[style]);
  Palette_LoadFromFile(data_021691a8, colour * 32, (palette + 16) << 5, 32);
}
