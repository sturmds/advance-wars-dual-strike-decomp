#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_022a7c5c.h"
#include "game/tile_sprite_task.h"
#include "sdk/fs.h"

extern char gPowerHighlightDir[];

u32 func_020e3554(int index);
u32 func_020e353c(int index);
u32 func_020e3524(int index);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

/* Loads the graphics and palette of a unit highlight effect from /bmap. On the
 * 3D screen they are queued to the data_022a7c5c destinations; on the 2D
 * screen the tiles go to object slot `slot` (32 bytes per tile) and the
 * palette to OBJ palette `palette`. */
void PowerHighlight_LoadGraphics(int effect, int color, int slot, int palette) {
  data_022a6c78 = 0;
  FS_ChangeDir(gPowerHighlightDir);
  if (data_027c027c == data_027c0288.unk_00) {
    Gfx_QueueUnpackedFileCopy((const char*)func_020e3554(effect),
                              data_022a7c5c->unk_68);
    Gfx_QueueFileCopy((const char*)func_020e3524(color), data_022a7c5c->unk_6c,
                      32);
  } else {
    File_UnpackTo(data_027c0280->objVram + slot * 32,
                  (const char*)func_020e353c(effect));
    Palette_LoadFromFile((const char*)func_020e3524(color), 0,
                         (u16)((palette + 16) << 5), 32);
  }
}
