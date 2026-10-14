#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Gfx_RequestLayerCopy(int bg);
void NumberFont_LoadLow(void);
void Co_LoadSmallPicture(int co, void* dest, int palette);
void Palette_LoadObjRow(int a);
void Co_UnpackPicture(int co, int a);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Palette_Load(const void* src, int offset, u32 size);
void Data_Unpack(const void* src, void* dest);
void Screen_SetBgMode(int a);
void Screen_InitLayers(const void* config);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void File_ReadAt(void* dest, const char* path, int a, int b);
u8* File_Load(const char* path);
void Mem_Free(void* res);
void Gfx_LoadBmap09f(void* a, int b, int c);
void Font_LoadDigits(int a);
void Icon_LoadTo(int a, u8* objVram, int b);
void IconBank_LoadPalette(int a, int b);
void Menu_Reset(void);
void Wallpaper_LoadSingle(void);
void ModeFont_ShowText(const void* text, int x, int y, int a, int b);
void ModeFont_Load(int x, int y);
void CampaignMap_MarkMissionDone(void);
void CampaignMap_FindTopCos(void);
BOOL Player1Team_HasPlayerInGame(void);
int Campaign_IsHardMode(void);
const char* File_LocalizePath(const char* path);
const void* Text_Get(u32 textId);
u8 Map_GetPlayerCount(u16 mapId);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern CampaignMapState gCampaignMapState;
extern MenuState gMenuState;
extern CampaignMission gCampaignMissions[];

extern u8 data_0215b548[];
extern u8 data_0215b598[];
extern char data_0215b990[]; /* "/ohashi/res_gmap" */
extern char data_0215b9a4[]; /* "/ohashi/res_gmap_lang" */
extern char data_0215b9bc[]; /* "/ohashi/res_icon_blink" */
extern char data_0215b9d4[]; /* "/ohashi/res_gmap_map1" */
extern char data_0215b9ec[]; /* "/ohashi/res_gmap_map2" */

/* Needed to match: the original adds these field offsets to the address
 * of gMenuState at run time instead of folding them into the
 * relocation. */
#define WORK_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->campaignMap.field)

/* Needed to match: mwcc does not fold the null base of this sum, so it
 * copies the index out of r0 with "add rN, r0, #0" as the original does.
 * A plain index is used straight from r0, and a "+ 0" on a local folds. */
#define UNFOLDED_INDEX(n) ((int)((u8*)0 + (n)))

/* Sets up both screens of the campaign world map (the res_gmap files):
 * layer priorities, blending and scroll, the background graphics, the
 * three top COs and the enemy COs of the selected mission. Text
 * 0x7d000014 or 0x7d000015 goes on top depending on Campaign_IsHardMode
 * (possibly hard campaign). */
void CampaignMap_Init(Unk_0204f960* task) {
  u8* res;
  u8* langRes;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;
  int objTile;
  int objTile2;
  int tile;
  int j;
  int playerCount;

  if (task->unk_52 == 2 && Player1Team_HasPlayerInGame()) {
    CampaignMap_MarkMissionDone();
  }
  CampaignMap_FindTopCos();
  /* COs of the map's last player slot (probably the enemy). */
  playerCount =
      Map_GetPlayerCount(gCampaignMissions[gCampaignMapState.mission].mapId);
  gMenuState.campaignMap.enemyCos[0] =
      Ov000_gMapHeaders[gCampaignMissions[gCampaignMapState.mission].mapId]
          .co[UNFOLDED_INDEX(playerCount)][0];
  playerCount =
      Map_GetPlayerCount(gCampaignMissions[gCampaignMapState.mission].mapId);
  gMenuState.campaignMap.enemyCos[1] =
      Ov000_gMapHeaders[gCampaignMissions[gCampaignMapState.mission].mapId]
          .co[UNFOLDED_INDEX(playerCount)][1];

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_0215b548);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 5;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
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

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_0215b598);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
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

  res = File_Load(data_0215b990);
  langRes = File_Load(File_LocalizePath(data_0215b9a4));

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Data_Unpack(
      res + 0x1ab0,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14) +
          0x4000);
  Data_Unpack(res + 0x36d4, data_027c0280->layer1Map);
  Palette_Load(res + 0x3ae8, 0x40, 0x20);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer1Map[i] += 0x2200;
  }
  Mem_Copy32(data_027c0280->layer1Map,
             data_027c0280->layer1Map + BG_SCREEN_ENTRIES, BG_SCREEN_SIZE);
  Gfx_RequestLayerCopy(1);
  Palette_Load(res + 0x3b08, 0x60, 0x140);
  Data_Unpack(res + 0xed8, data_027c0280->objVram);
  Palette_Load(res + 0x1884, 0x200, 0x40);
  Data_Unpack(res + 0x848, data_027c0280->objVram + 0x2080);
  Data_Unpack(res + 0xdcc, data_027c0280->objVram + 0x2f80);
  Data_Unpack(res, data_027c0280->objVram + 0x3140);
  Palette_Load(res + 0x834, 0x240, 0x20);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Data_Unpack(
      langRes,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Data_Unpack(res + 0x18c4, data_027c0280->layer2Map);
  Palette_Load(res + 0x1a70, 0x20, 0x40);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer2Map[i] += 0x1000;
  }
  Gfx_RequestLayerCopy(2);
  Data_Unpack(res + 0x1734, data_027c0280->objVram);
  Palette_Load(res + 0x1884, 0x200, 0x20);
  for (i = 0, objTile = 38; i < 3; i++, objTile += 8) {
    Icon_LoadTo(i + 0xbc, data_027c0280->objVram, objTile);
  }
  IconBank_LoadPalette(4, 18);
  for (i = 0, objTile2 = 158; i < 4; i++, objTile2 += 4) {
    Icon_LoadTo(i + 0x55, data_027c0280->objVram, objTile2);
  }
  Icon_LoadTo(0xc6, data_027c0280->objVram, 174);
  IconBank_LoadPalette(1, 19);
  for (i = 0, tile = 62, j = 98; i < 3; i++, tile += 12, j += 12) {
    if (gMenuState.campaignMap.topCos[i].co != 0) {
      Co_LoadSmallPicture(gMenuState.campaignMap.topCos[i].co,
                          data_027c0280->objVram + ((u32)tile << 5), i + 20);
      Co_UnpackPicture(gMenuState.campaignMap.topCos[i].co, j);
    }
  }
  Palette_LoadObjRow(1);
  for (j = 0, tile = 134; j < 2; j++, tile += 12) {
    if (gMenuState.campaignMap.enemyCos[j] != 0) {
      Co_LoadSmallPicture(gMenuState.campaignMap.enemyCos[j],
                          data_027c0280->objVram + ((u32)tile << 5), j + 23);
    }
  }
  Screen_SetCurrentGfx(saved0280);

  Mem_Copy32(res + 0xe4c, WORK_FIELD(unk_244),
             sizeof(gMenuState.campaignMap.unk_244));
  Mem_Copy32(res + 0xd08, WORK_FIELD(unk_020),
             sizeof(gMenuState.campaignMap.unk_020));
  Mem_Copy32(res + 0x6d4, WORK_FIELD(unk_0e4),
             sizeof(gMenuState.campaignMap.unk_0e4));
  Mem_Free(res);
  Mem_Free(langRes);
  File_ReadAt(gMenuState.colours, data_0215b9bc, 0, 0x20);

  res = File_Load(data_0215b9d4);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(
      res, data_027c0280->bgVram +
               (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Data_Unpack(res + 0x53bc, data_027c0280->layer2Map);
  for (i = 0; i < BG_SCREEN_ENTRIES; i++) {
    data_027c0280->layer2Map[i] += 0x3000;
  }
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(res);

  res = File_Load(data_0215b9ec);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(
      res, data_027c0280->bgVram +
               (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
  Data_Unpack(res + 0x4c7c, data_027c0280->layer3Map + 0x400);
  for (i = 0; i < BG_SCREEN_ENTRIES; i++) {
    data_027c0280->layer3Map[i + 0x400] += 0x3000;
  }
  Gfx_RequestLayerCopy(3);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(res);

  ModeFont_Load(178, 9);
  if (Campaign_IsHardMode() == 0) {
    ModeFont_ShowText(Text_Get(0x7d000014), 178, 9, 0, 0);
  } else {
    ModeFont_ShowText(Text_Get(0x7d000015), 178, 9, 0, 0);
  }
  Wallpaper_LoadSingle();
  NumberFont_LoadLow();
  Menu_Reset();
}
