#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/link_buffers.h"
#include "game/event_queue.h"
#include "game/icon.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_0229ff30.h"

void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Mem_Free(void* file);
void Mem_Copy32(const void* src, void* dst, u32 size);
void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Palette_LoadObjRow(int row);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int baseTile);
void Co_UnpackPicture(int coId, int firstTile);
void CombatSetup_LoadMapPreviews(void);
void NumberFont_LoadLow(void);
void TpIcon_Load(void);
void TpIcon2_LoadTo(u8* dest);
u16 Wireless_GetChildAids(void);
int func_0207a260(void);
void SpriteTask_Create(int a, int b, int c, int d);
void Menu_Reset(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void Wallpaper_LoadBoth(void);
void ModeFont_Load(int tile, int paletteSlot);
const char* Text_Get(u32 textId);
void ModeFont_ShowText(const char* text, int tile, int paletteSlot, int a,
                       int b);
void HelpBand_Start(int tile, int a);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern u8 data_02160ae0[]; /* the seven CO ids of the Combat setup screens */
extern const char gCombatLinkCommonLangResPath[];
extern const char gCombatLinkCommonResPath[];
extern const char gCombatLinkResPath[];
extern const char gCombatLinkLangResPath[];
extern const char gCombatSetupMenuResPath[];

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

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

/* Sets up the Combat setup screen (link play): both screens' graphics from
 * /commo/res_linkcommon, /combat_st/res_cmlink and res_cmmenu, the small
 * pictures of the seven COs, the sub screen as a copy of the main screen's
 * sprites and layer 2 tiles, then the saved maps and row palettes in
 * MenuState. Then it reads the mask of link players (bit 0 always set) and
 * the local player, deals the players out to rows 1-4, and starts the
 * menu, the mode title (0x7d000013) and the help band. */
void CombatSetup_Init(void) {
  int team;
  int i;
  u8* file;
  u8* langFile;
  u16* map;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  Gfx_InitBothScreens();

  SWITCH_TO(MAIN_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  SWITCH_BACK();

  file = File_Load(File_LocalizePath(gCombatLinkCommonLangResPath));
  Data_Unpack(file, data_027c0280->objVram + 0x1e00);
  Mem_Free(file);
  file = File_Load(gCombatLinkCommonResPath);
  Mem_Copy32(file + 32, MENU->combat.unk_1400, sizeof(MENU->combat.unk_1400));
  Mem_Free(file);

  file = File_Load(gCombatLinkResPath);
  langFile = File_Load(File_LocalizePath(gCombatLinkLangResPath));

  SWITCH_TO(MAIN_MAP);
  Font_LoadDigits(0);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Data_Unpack(langFile, data_027c0280->objVram);
  Palette_Load(file, 0x200, 32);
  Palette_Load(file, 0x220, 32);
  for (i = 0; i < COMBAT_CO_COUNT; i++) {
    Co_UnpackPicture(data_02160ae0[i], 0x4c0 + i * 12);
  }
  Palette_LoadObjRow(11);
  Data_Unpack(
      file + 32,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Palette_Load(file + 0x7f0, 0x40, 0x80);
  Icon_LoadTo(61, data_027c0280->objVram, 0x638);
  Icon_LoadPalette(61, 26);
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  Font_LoadDigits(0);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Mem_Copy32(((Unk_027c0280*)MAIN_MAP)->objVram, data_027c0280->objVram,
             0x2580);
  Palette_Load(file, 0x200, 32);
  Palette_Load(file, 0x220, 32);
  Mem_Copy32(((Unk_027c0280*)MAIN_MAP)->unk_10c,
             data_027c0280->bgVram +
                 (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
             0x840);
  Palette_Load(file + 0x7f0, 0x40, 0x80);
  CombatSetup_LoadMapPreviews();
  SWITCH_BACK();

  /* The sub screen's layer 2 map, kept at the start of MenuState
     * (CombatSetup_InitScreen copies it); 0x2000 adds 2 to each tile's
     * palette number. */
  map = (u16*)&gMenuState;
  Data_Unpack(file + 0x3b8, map);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    *map += 0x2000;
    map++;
  }
  map = MENU->combat.unk_600;
  Data_Unpack(file + 0x554, map);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    *map += 0x2000;
    map++;
  }
  Mem_Free(langFile);
  Mem_Free(file);

  file = File_Load(gCombatSetupMenuResPath);
  SWITCH_TO(MAIN_MAP);
  Data_Unpack(file + 0x1cc, data_027c0280->objVram + 0xa280);
  Data_Unpack(file + 0x12cc, data_027c0280->objVram + 0xc480);
  SWITCH_BACK();
  Mem_Copy32(file + 0x14f4, MENU->combat.rowPalettes,
             sizeof(MENU->combat.rowPalettes));
  Mem_Copy32(file + 0x16d4, MENU->combat.rowObjPalettes,
             sizeof(MENU->combat.rowObjPalettes));
  Data_Unpack(file + 0xdc, MENU->combat.savedMap);
  Mem_Free(file);

  NumberFont_LoadLow();
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0x2580);
  data_0229ff30.playerMask = Wireless_GetChildAids() | 1;
  data_0229fb30.unk_45a = func_0207a260();
  data_0229fb30.unk_45b = 0;
  EventQueue_Reset();

  for (i = 1; i < COMBAT_ROW_COUNT + 1; i++) {
    MENU->combat.rows[i - 1].memberCount = 0;
    MENU->combat.rows[i - 1].locked = 0;
  }
  for (i = 0; i < (int)ARRAY_COUNT(MENU->combat.players); i++) {
    MENU->combat.players[i].row = 0;
    MENU->combat.players[i].ready = 0;
  }
  /* Deal the link players out to the four rows in turn. A row keeps
     * its players in members, memberCount of them. */
  team = 1;
  for (i = 0; i < LINK_AID_COUNT; i++) {
    if (data_0229ff30.playerMask & (1 << i)) {
      MENU->combat.rows[team - 1]
          .members[MENU->combat.rows[team - 1].memberCount] = i;
      MENU->combat.rows[team - 1].memberCount++;
      MENU->combat.players[i].row = team;
      team++;
      if (team == 5) {
        team = 1;
      }
    }
  }

  SpriteTask_Create(232, 8, 348, 15);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 0x100, -16);
  Wallpaper_LoadBoth();
  ModeFont_Load(364, 8);
  ModeFont_ShowText(Text_Get(0x7d000013), 364, 8, 5, 0);
  HelpBand_Start(0x3f8, 0);
}
