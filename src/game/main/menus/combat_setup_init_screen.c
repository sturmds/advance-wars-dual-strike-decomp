#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Gfx_RequestLayerCopy(int layer);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Menu_DrawFourLabels(void);
void Palette_Flush(void);
void Palette_Load(const void* src, u32 offset, u32 size);
void func_0200208c(int coId, void* dest, int paletteSlot, int army);
void HelpText_Show(u32 textId, int tile);
void CombatSetup_DrawShopHelp(void);
void CombatSetup_DrawShop(void);
void func_02088528(int a0);

extern MenuState gMenuState;
extern u8 data_02160ae0[];
extern const MenuButtonTemplate data_02160e08;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* The graphics state of the main screen, while the sub screen is current. */
#define MAIN_GFX ((Unk_027c0280*)data_027c0288.unk_00)

/* Sets up the Combat setup screen for the local player's row (link play).
 * Main screen: the saved layer 1 map, the row's palettes and the small CO
 * picture of each row with memberCount set (palette 18 + row). Sub screen: the
 * saved layer 2 map, scrolled by -16, and copies of those pictures and
 * palettes. Then it resets every row's choice to button 22, draws the texts,
 * adds button 0xfd without touch, and prints help text 0x44000013. */
void CombatSetup_InitScreen(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  Unk_027c0280* subSavedGfx;
  int row;
  int i;
  int player;

  row = MENU->combat.players[data_0229fb30.unk_45a].row;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Mem_Copy32(MENU->combat.savedMap, data_027c0280->layer1Map,
             sizeof(MENU->combat.savedMap));
  Gfx_RequestLayerCopy(1);
  Palette_Load(MENU->combat.rowPalettes[row - 1], 0x2c0, 0x60);
  Palette_Load(MENU->combat.rowObjPalettes[row - 1], 0x320, 32);
  for (i = 0; i < COMBAT_ROW_COUNT; i++) {
    if (MENU->combat.rows[i].memberCount != 0) {
      func_0200208c(data_02160ae0[LINK_ROWS[i].unk_19],
                    data_027c0280->objVram + (0x490 + i * 12) * 32, i + 18,
                    i + 1);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  subSavedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Mem_Copy32(&gMenuState, data_027c0280->layer2Map, BG_VISIBLE_SIZE);
  Gfx_RequestLayerCopy(2);
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = -16;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = -16;
  for (i = 0; i < COMBAT_ROW_COUNT; i++) {
    Mem_Copy32(MAIN_GFX->objVram + (0x490 + i * 12) * 32,
               data_027c0280->objVram + (300 + i * 12) * 32, 0x180);
    Mem_Copy32(MAIN_GFX->paletteBuffer + (i + 18) * 32,
               data_027c0280->paletteBuffer + (i + 20) * 32, 32);
  }
  Palette_Flush();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(subSavedGfx);

  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Select(22);
  for (i = 1; i < COMBAT_ROW_COUNT + 1; i++) {
    MENU->combat.rows[i - 1].buttonId = 22;
  }
  CombatSetup_DrawShop();
  Menu_DrawFourLabels();
  CombatSetup_DrawShopHelp();
  MenuButton_Add(0, 0, &data_02160e08, 0xfd);
  MenuButton_DisableTouch(0xfd);
  /* The local player does not own its row: func_02088528 gets the owner. */
  player = data_0229fb30.unk_45a;
  if (player !=
      MENU->combat.rows[MENU->combat.players[player].row - 1].player) {
    func_02088528(
        MENU->combat.rows[MENU->combat.players[player].row - 1].player);
  }
  HelpText_Show(0x44000013, 960);
}
