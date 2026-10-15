#include "types.h"
#include "game/screen_other_map.h"
#include "game/layer0_picture_task.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void Gfx_RequestLayerCopy(int layer);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);

extern char data_02168984[];
extern char data_0216898c[];

/* Loads a picture into BG layer 0: tiles from a fixed file, the tile map
 * and palette from the task's two files. */
void Layer0Picture_Load(Layer0PictureTask* task) {
  FS_ChangeDir(data_02168984);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14) +
          0x5100,
      data_0216898c);
  File_UnpackTo(data_027c0280->layer0Map, task->mapFile);
  /* adds tile 0x288 and palette 1 to every entry of the map */
  TileMap_AddToEntries(data_027c0280->layer0Map, 0x800, 0x1288);
  Palette_LoadFromFile(task->paletteFile, 0, 32, 32);
  Gfx_RequestLayerCopy(0);
}
