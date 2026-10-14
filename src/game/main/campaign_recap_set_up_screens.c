#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/campaign_recap.h"

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
void CampaignRecap_LoadTitleCallSprites(CampaignRecap* work);
void CampaignRecap_FillEntries(CampaignRecap* work);
void Battle_SetupWithPresetCos(
    CampaignRecap*
        work); /* needed to match: the call passes work in r0, which the callee ignores */
Task* Sound_StartSongSwitch(
    int song, int arg); /* task: fade out the song, then start `song` */

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_0216a8d8[];         /* layer config for Screen_InitLayers */
extern const char data_0216a928[]; /* "/rikiishi" */
extern const char data_0216a934[]; /* "013": layer 2 tiles */
extern const char data_0216a938[]; /* "014": layer 2 map */
extern const char data_0216a93c[]; /* "015": layer 2 palette */
/* map bytes of a text BG by screen size */
static const u32 data_0214d3e0[4] = {
#include "data/main/data_0214d3e0.inc.c"
};
/* ADPCM step index change per nibble */
const s8 data_0214d400[16] = {
#include "data/main/data_0214d400.inc.c"
};
/* map bytes of a rotation/scaling BG by screen size */
static const u32 data_0214d3f0[4] = {
#include "data/main/data_0214d3f0.inc.c"
};
/* ADPCM step sizes */
const s16 data_0214d410[89] = {
#include "data/main/data_0214d410.inc.c"
};
/* a sine and cosine pair per 16 angle units */
const s16 data_0214d4c4[4096] = {
#include "data/main/data_0214d4c4.inc.c"
};

/* Task step of data_0216a8b0, between the work allocation (CampaignRecapTask_Init)
 * and CampaignRecap_Update. Sub screen (data_027c0288.unk_04): BG2 and OBJ
 * on, layer priorities 0 to 3, alpha blending of BG2 over the rest, layer 2
 * tiles, map (palette 8) and palette from /rikiishi 013-015, then the
 * res_titlecall sprites (CampaignRecap_LoadTitleCallSprites). Main screen: BG0 only, no blending,
 * the preset-CO battle setup. Then it clears the recap work, starts song 42
 * and blacks out both screens. */
void CampaignRecap_SetUpScreens(CampaignRecapTask* task) {
  CampaignRecap* work = task->work;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_0216a8d8);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
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
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    u16* map;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    FS_ChangeDir(data_0216a928);
    File_UnpackTo(
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
        data_0216a934);
    File_UnpackTo(data_027c0280->layer2Map, data_0216a938);
    map = data_027c0280->layer2Map;
    TileMap_AddToEntries(
        map,
        Screen_IsTextBg(2)
            ? data_0214d3e0[GFX_LAYER(data_027c0280, 2)->screenSize]
        : Screen_IsAffineBg(2)
            ? data_0214d3f0[GFX_LAYER(data_027c0280, 2)->screenSize]
            : data_0214d3f0[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
        0x8000);
    Palette_LoadFromFile(data_0216a93c, 0, 0x100, 0x100);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  CampaignRecap_LoadTitleCallSprites(work);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Battle_SetupWithPresetCos(work);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  work->blend[0] = 0;
  work->blend[1] = 0;
  work->blend[2] = 0;
  work->slide[0] = 0;
  work->slide[1] = 0;
  work->slide[2] = 0;
  work->number = 0;
  work->mapOffset = 0;
  work->unk_20 = 0;
  work->unk_30 = 0;
  work->index = 0;
  work->firstStep = 3;
  CampaignRecap_FillEntries(work);
  work->songFadeTask = Sound_StartSongSwitch(42, 0);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->masterBrightness = -16;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->masterBrightness = -16;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  task->step = 0;
}
