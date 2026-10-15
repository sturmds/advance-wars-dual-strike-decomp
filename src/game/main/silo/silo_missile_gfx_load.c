#include "types.h"
#include "game/Unk_02088d04.h"
#include "sdk/fs.h"

extern char data_02169180[]; /* "/bmap" */
extern char data_021691c0[]; /* "065" */
extern char data_021691c4[]; /* "066" */

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          u16 paletteOffset, int size);

/* Loads the silo missile graphics (/bmap/065) to sprite tile `tile` and
 * its colours (/bmap/066) to sprite palette `palette`. */
void SiloMissile_LoadGfx(int tile, int palette) {
  FS_ChangeDir(data_02169180);
  File_UnpackTo(data_027c0280->objVram + tile * 32, data_021691c0);
  Palette_LoadFromFile(data_021691c4, 0, (palette + 16) << 5, 32);
}
