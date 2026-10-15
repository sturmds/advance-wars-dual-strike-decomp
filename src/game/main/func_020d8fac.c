#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "sdk/fs.h"

extern char data_021681d8[];
extern char data_021681f8[];
extern char data_021681fc[];
extern char data_02168200[];

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);

/* Loads a graphics file and its palette from the data_021681d8 directory:
 * into the buffers of data_022a7c5c on the screen that shows the 3D engine,
 * into sprite VRAM and the palette buffer on the other. Name
 * kept: what the graphics show is unknown. */
void func_020d8fac(void) {
  if (data_027c027c == data_027c0288.unk_00) {
    FS_ChangeDir(data_021681d8);
    Gfx_QueueUnpackedFileCopy(data_021681f8, data_022a7c5c->unk_68);
    Gfx_QueueFileCopy(data_021681fc, data_022a7c5c->unk_6c, 32);
  } else {
    FS_ChangeDir(data_021681d8);
    File_UnpackTo(data_027c0280->objVram + 0x3280, data_02168200);
    Palette_LoadFromFile(data_021681fc, 0, 672, 32);
  }
}
