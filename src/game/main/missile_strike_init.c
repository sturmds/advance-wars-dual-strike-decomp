#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/missile_strike_task.h"
#include "game/screen.h"
#include "sdk/fs.h"

extern char data_02169180[]; /* "/bmap" */
extern char data_02169188[]; /* "089" */
extern char data_0216918c[]; /* "08a" */
extern char data_02169190[]; /* "08b" */
extern char data_02169194[]; /* "08c" */
extern char data_02169198[]; /* "08d" */

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void File_UnpackTo(void* dest, const char* fileName);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);

/* Tile 0x288 (0x5100 bytes into the character base), palette 1. */
#define BG_TILE_BASE 0x1288

/* Script step of the missile strike task: loads its BG picture (/bmap/089
 * tiles, 08a map, 08b palette 1) into layer 0 and its sprite graphics
 * (08c, palette 08d), sets the strike's pixel position from its tile, and
 * alpha-blends layer 0 over the other layers and the sprites at EVA 16,
 * EVB 16. */
void MissileStrike_Init(MissileStrikeTask* task) {
  Screen_SetLayerScreenSize(0, 2);
  Gfx_ClearLayerMap(0);
  Gfx_RequestLayerCopy(0);
  FS_ChangeDir(data_02169180);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14) +
          0x5100,
      data_02169188);
  File_UnpackTo(data_027c0280->layer0Map, data_0216918c);
  TileMap_AddToEntries(data_027c0280->layer0Map, 0x800, BG_TILE_BASE);
  Palette_LoadFromFile(data_02169190, 0, 32, 32);
  Gfx_RequestLayerCopy(0);
  Gfx_QueueUnpackedFileCopy(data_02169194, data_022a7c5c->unk_68);
  Gfx_QueueFileCopy(data_02169198, data_022a7c5c->unk_6c, 32);

  task->pixelX = task->tileX * 16 + 8;
  task->pixelY = task->tileY * 16 + 16;
  task->unk_48 = 0;
  task->fallHeight = task->pixelY;

  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
}
