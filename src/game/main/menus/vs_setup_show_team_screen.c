#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/task.h"

/* Loads the picture of a CO (id in the low 7 bits; 0 clears dest) to dest,
 * 18 tiles, and its palette in the colours of the given army. */
void Co_LoadArmyMediumPicture(int co, void* dest, int paletteSlot, int army);
void Screen_SetCurrentBoth(void* state);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void VsSetup_DrawTeamSlotPanels(void);
void VsSetup_DrawSubSlotPanels(void);
void VsSetup_AddTeamArrowButtons(void);
void HelpText_Show(u32 textId, int tile);
void Task_JumpToLabel(Task* task, int label);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* CO pictures of the player slots: 18 tiles and one
 * OBJ palette each, front CO then back CO. */
#define CO_PICTURE_TILE(n) (552 + (n) * 18)
#define CO_PICTURE_PALETTE(n) (18 + (n))

/* Task step that opens the team screen of the Vs setup: loads the front
 * and back CO pictures of each player slot, then with two players jumps to
 * label 255 of the task's script (no teams to pick). Otherwise it runs
 * Gfx_ClearLayerMap and Gfx_RequestLayerCopy on layers 0-2 of both screens,
 * selects the slot button (48 + slot) of the
 * slot under the setup cursor, adds the team arrow buttons and shows the
 * help text. */
void VsSetup_ShowTeamScreen(Task* task) {
  int player;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (player = 0; player < MENU->setup.unk_2e3; player++) {
    if (MENU->setup.cos[player][0] != 0) {
      Co_LoadArmyMediumPicture(
          MENU->setup.cos[player][0],
          data_027c0280->objVram + CO_PICTURE_TILE(player * 2) * 32,
          CO_PICTURE_PALETTE(player * 2), player + 1);
    }
    if (MENU->setup.cos[player][1] != 0) {
      Co_LoadArmyMediumPicture(
          MENU->setup.cos[player][1],
          data_027c0280->objVram + CO_PICTURE_TILE(player * 2 + 1) * 32,
          CO_PICTURE_PALETTE(player * 2 + 1), player + 1);
    }
  }
  Screen_SetCurrentGfx(saved0280);
  if (MENU->setup.unk_2e3 == 2) {
    Task_JumpToLabel(task, 255);
    return;
  }

  savedGfx = data_027c0280;
  savedScreen = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_ClearLayerMap(2);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_ClearLayerMap(2);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  VsSetup_DrawTeamSlotPanels();
  VsSetup_DrawSubSlotPanels();
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 0x100, -16);
  /* Cursor ids 0-7 are the map buttons, 8-15 the CO buttons, two per slot. */
  if (MENU->setup.cursorButton < 8) {
    MENU->setup.selectedSlot = MENU->setup.cursorButton >> 1;
  } else {
    MENU->setup.selectedSlot = (MENU->setup.cursorButton - 8) >> 1;
  }
  MenuButton_Select(MENU->setup.selectedSlot + 48);
  VsSetup_AddTeamArrowButtons();
  MenuButtons_DisableTouch();
  HelpText_Show(0x7f000017, 960);
}
