#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb84.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov005_02368378[];       /* main screen layer config */
extern char data_ov005_02368a08[];     /* "/rikiishi" */
extern char gRikiishiFile022[];
extern char gRikiishiFile023[];
extern char gRikiishiFile024[];
extern char gRikiishiFile025[];
extern char gRikiishiFile026[];
extern u32 data_ov005_02367f2c[];      /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367f3c[];      /* map bytes of a rotation/scaling BG by screen size */

/* Screen setup step of an overlay 5 screen (task script data_ov005_023682e0).
 * Main screen only: mode 0 with BG0, BG2, BG3 and OBJ on, layer priorities
 * 0 to 3, no windows or blending. Layers 2 and 3 come from /rikiishi files
 * 022 to 026 and sit 8 pixels lower; layer 2 then starts 256 pixels to the
 * side, so a later step can slide it in. */
void Ov005_SetupMainScreenLayers2And3(Ov005Task *task)
{
    Unk_ov005_0236bb84 *work;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    work = task->work;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_SetBgMode(0);
    Screen_InitLayers(data_ov005_02368378);
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt &= ~0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
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
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = -8;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = -8;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    /* needed to match: the file block gets its own locals */
    {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c;
        u16 *map;

        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        FS_ChangeDir(data_ov005_02368a08);
        File_UnpackTo(data_027c0280->bgVram
                          + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
                      gRikiishiFile022);
        File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile023);
        map = data_027c0280->layer2Map;
        TileMap_AddToEntries(map,
                      func_01ff8ef8(2) ? data_ov005_02367f2c[GFX_LAYER(data_027c0280, 2)->screenSize]
                      : Screen_IsAffineBg(2) ? data_ov005_02367f3c[GFX_LAYER(data_027c0280, 2)->screenSize]
                                             : data_ov005_02367f3c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                      0x6000);
        Gfx_RequestLayerCopy(2);
        File_UnpackTo(data_027c0280->bgVram
                          + (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14),
                      gRikiishiFile024);
        File_UnpackTo(data_027c0280->layer3Map, gRikiishiFile025);
        map = data_027c0280->layer3Map;
        TileMap_AddToEntries(map,
                      func_01ff8ef8(3) ? data_ov005_02367f2c[GFX_LAYER(data_027c0280, 3)->screenSize]
                      : Screen_IsAffineBg(3) ? data_ov005_02367f3c[GFX_LAYER(data_027c0280, 3)->screenSize]
                                             : data_ov005_02367f3c[GFX_LAYER(data_027c0280, 3)->screenSize] * 2,
                      0x6000);
        Gfx_RequestLayerCopy(3);
        Palette_LoadFromFile(gRikiishiFile026, 0, 0xc0, 0x140);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }

    task->step = 0;
    work->unk_04 = 0;
    work->unk_1c = 0;
    work->unk_20 = 2000;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 256;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
