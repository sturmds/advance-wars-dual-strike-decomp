#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/map_header.h"
#include "game/design_map.h"
#include "game/event_queue.h"
#include "game/text_print.h"
#include "game/link_slots.h"
#include "game/screen_other_map.h"

void Gfx_RequestLayerCopy(int a);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Data_Unpack(const void* src, void* dst);
void* File_Load(const char* path);
void Mem_Free(void* file);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Window_DrawOnLayer1(int x, int y, int w, int h);
void Font_LoadDigits(int a);
void Bg_PutTileBlock(void* target, int a, int b, const void* src, int c);
void Palette_BlendWithColor(void* dst, int a, int b, int c);
void Gfx_InitBothScreens(void);
void Wallpaper_LoadBoth(void);
void ModeFont_ShowText(const void* text, int a, int b, int c, int d);
void ModeFont_Load(int a, int b);
void MapPreview_Draw(void* dst, int mapId);
void PaletteCycle_Start(int a);
void LinkScreen_AddButtons(void);
int LinkScreen_GetPlayerRow(int player);
int Link_IsActive(void);
void SpriteTask_Create(int a, int b, int c, int d);
const char* File_LocalizePath(const char* path);
u8 Map_GetPlayerCount(u32 mapId);
const void* Map_GetName(u32 mapId);
const void* Text_Get(u32 textId);

extern DesignMap data_022a6c84[];
extern u16 gLinkPhaseArmyGfxOffsets[];
extern const char gLinkPhaseResPath[];
extern const char gLinkCommonLangResPath[];
extern const char gLinkCommonResPath[];

/* data_027c0288 holds two game states; Screen_SetCurrentGfx and Screen_SetCurrent load
 * one into data_027c0280 and data_027c027c. */
#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

#define LINK_STATE ((MenuState*)&gMenuState)

/* Message ids; which one shows depends on MAP_STATE->twoMaps. */
#define TEXT_LINK_ONE_MAP 0x7d000017
#define TEXT_LINK_TWO_MAPS 0x7d000018

/* Sets up the screen of the wireless link phase (graphics from
 * /ohashi/res_linkphase and /commo/res_linkcommon). Copies each player's
 * army colour from the current map into gMenuState, draws one army icon
 * per player, and writes the map name on the second game state's screen.
 * When MAP_STATE->twoMaps is 1 it writes the second map's name there, and the
 * main state's screen also gets the first map's name. */
void LinkPhase_InitScreen(void) {
  int i;
  u8* file;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int mapId; /* Map_GetName gets it as (u16), needed to match */
  BOOL customMap;
  void*
      target; /* needed to match: read before the call in the same argument list */

  gMenuState.link.playerCount = Map_GetPlayerCount(MAP_STATE->unk_1c4->mapId);
  /* Needed to match: the flag is kept in a register, and the stores go
     * through a cast pointer so the compiler reloads data_027c027c and
     * data_027c0284 on every pass. */
  customMap = MAP_IS_CUSTOM(MAP_STATE->unk_1c4->mapId);
  if (customMap) {
    for (i = 0; i < gMenuState.link.playerCount; i++) {
      LINK_STATE->link.armyColours[i] =
          data_022a6c84[MAP_STATE->unk_1c4->mapId - MAP_CUSTOM_FIRST]
              .armyColours[i + 1];
    }
  } else {
    for (i = 0; i < gMenuState.link.playerCount; i++) {
      LINK_STATE->link.armyColours[i] =
          Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId].army[i + 1];
    }
  }
  for (i = 0; i < (int)ARRAY_COUNT(gMenuState.link.panelPlayers); i++) {
    gMenuState.link.panelPlayers[i] = gLinkSlotPlayers[i] - 1;
    gMenuState.link.readyFlags[i] = 0;
  }
  Gfx_InitBothScreens();

  SWITCH_TO(MAIN_MAP);
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 10;
  data_027c0280->blendEvb = 6;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 1;
  SWITCH_BACK();

  file = File_Load(gLinkPhaseResPath);
  SWITCH_TO(MAIN_MAP);
  Data_Unpack(
      file, data_027c0280->bgVram +
                (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Palette_Load(file + 0x2a4, 0, 0xa0);
  for (i = 0; i < gMenuState.link.playerCount; i++) {
    target = data_027c0280->layer2Map;
    Bg_PutTileBlock(
        target, 4, LinkScreen_GetPlayerRow(i),
        file + gLinkPhaseArmyGfxOffsets[gMenuState.link.armyColours[i] - 1], 0);
  }
  Gfx_RequestLayerCopy(2);
  SWITCH_BACK();
  Mem_Free(file);

  file = File_Load(File_LocalizePath(gLinkCommonLangResPath));
  Data_Unpack(file, data_027c0280->objVram);
  Mem_Free(file);

  file = File_Load(gLinkCommonResPath);
  Palette_Load(file, 0x200, 0x20);
  Palette_Load(file, 0x220, 0x20);
  Palette_Load(file, 0x240, 0x20);
  Palette_BlendWithColor(data_027c0280->paletteBuffer + 0x240, 0, 8, 8);
  Mem_Copy32(file + 0x20, gMenuState.colours, sizeof(gMenuState.colours));
  Mem_Free(file);

  if (MAP_STATE->twoMaps == 1) {
    mapId = MAP_STATE->secondMapId;
  } else {
    mapId = MAP_STATE->unk_1c4->mapId;
  }
  SWITCH_TO(SECOND_MAP);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Window_DrawOnLayer1(11, 20, 10, 4);
  target = data_027c0280->layer0Map;
  Text_Print(12, 21, target, Map_GetName((u16)mapId), 0, 0x100);
  MapPreview_Draw(data_027c0280->objVram + 0x4000, mapId);
  PaletteCycle_Start(18);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  SWITCH_BACK();

  if (MAP_STATE->twoMaps == 1) {
    mapId = MAP_STATE->unk_1c4->mapId;
    SWITCH_TO(MAIN_MAP);
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    Window_DrawOnLayer1(11, 20, 10, 4);
    target = data_027c0280->layer0Map;
    Text_Print(12, 21, target, Map_GetName((u16)mapId), 0, 0x100);
    MapPreview_Draw(data_027c0280->objVram + 0x780, mapId);
    PaletteCycle_Start(19);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    SWITCH_BACK();
  }

  ModeFont_Load(0, 0);
  if (MAP_STATE->twoMaps == 0) {
    ModeFont_ShowText(Text_Get(TEXT_LINK_ONE_MAP), 0, 0, 1, 0);
  } else {
    ModeFont_ShowText(Text_Get(TEXT_LINK_TWO_MAPS), 0, 0, 1, 0);
  }
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 0x100, -16);
  MenuButton_Select(0);
  LinkScreen_AddButtons();
  MenuButtons_DisableTouch();
  Wallpaper_LoadBoth();
  EventQueue_Reset();
  if (Link_IsActive() == 1) {
    SpriteTask_Create(0xe8, 8, 0x1f0, 15);
  }
}
