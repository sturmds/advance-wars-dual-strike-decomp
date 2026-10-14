#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern GfxLayer data_ov005_0236966c[]; /* layer config for Screen_InitLayers */
extern char data_ov005_0236988c[];     /* directory of the files below */
extern char data_ov005_02369898[];     /* layer 2 tiles */
extern char data_ov005_0236989c[];     /* layer 2 map */
extern char data_ov005_023698a0[];     /* layer 2 palette */
/* map bytes of a text BG by screen size */
const u32 data_ov005_02367f8c[] = {
#include "data/ov005/data_ov005_02367f8c.inc.c"
};
/* map bytes of a rotation/scaling BG by screen size */
const u32 data_ov005_02367f9c[] = {
#include "data/ov005/data_ov005_02367f9c.inc.c"
};

/* Screen setup step of the large-work screen (task script data_ov005_02369634):
 * on the main screen (data_027c0288.unk_00), only BG2 on, layer priorities 0
 * to 3, no blending or windows, no scroll, then the tiles, map and palette of
 * layer 2. Resets task->step to 0 for the next update function. */
void Ov005_SetUpMainScreenLayer2(Ov005Task* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov005_0236966c);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  /* needed to match: the file loading gets its own locals */
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    u16* map;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    FS_ChangeDir(data_ov005_0236988c);
    File_UnpackTo(
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
        data_ov005_02369898);
    File_UnpackTo(data_027c0280->layer2Map, data_ov005_0236989c);
    map = data_027c0280->layer2Map;
    TileMap_AddToEntries(
        map,
        Screen_IsTextBg(2)
            ? data_ov005_02367f8c[GFX_LAYER(data_027c0280, 2)->screenSize]
        : Screen_IsAffineBg(2)
            ? data_ov005_02367f9c[GFX_LAYER(data_027c0280, 2)->screenSize]
            : data_ov005_02367f9c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
        0);
    Palette_LoadFromFile(data_ov005_023698a0, 0, 0, 32);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  task->step = 0;
}
