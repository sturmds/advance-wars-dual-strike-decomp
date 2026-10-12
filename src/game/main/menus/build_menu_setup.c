#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/build_menu.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern MenuButtonTemplate data_02158f54;

void ScreenConfig_RunActionHook(void);
void Font_LoadDigits(int baseTile);
void func_020c4018(void);
void BuildMenu_DrawColumnWithButtons(int x, int y, int first);
void Sound_PlaySe(int id);
void UnitInfoPanel_OpenForTypeOnSecondScreen(int owner, int unitType);
void SidePanel_Place(int mode);

/* A step of the build menu task (Task_Create with data_0216594c): selects the
 * first row, draws the unit list (one column centred, or two columns of
 * BUILD_MENU_ROWS once there are more than 8 entries), plays sound 0x1000a
 * and shows the first unit's info on the other screen. */
void BuildMenu_Setup(Task* task) {
  u8 bg;

  ScreenConfig_RunActionHook();
  Font_LoadDigits(0);
  func_020c4018();
  bg = data_027c0280->layer0Bg;
  data_027c0280->bgScrollX[bg] = 0;
  data_027c0280->bgScrollY[bg] = data_027c0280->bgScrollX[bg];
  task->waitFrames = 0;
  BUILD_MENU_VARS(task)->unk_2c = 0;
  BUILD_MENU_VARS(task)->lastEntry = gBuildMenuCount - 1;
  MenuButton_Select(0);
  if (BUILD_MENU_VARS(task)->lastEntry <= BUILD_MENU_ROWS) {
    BuildMenu_DrawColumnWithButtons(7, 6, 0);
  } else {
    BuildMenu_DrawColumnWithButtons(0, 6, 0);
    BuildMenu_DrawColumnWithButtons(16, 6, BUILD_MENU_ROWS);
  }
  MenuButton_AddReserved(0, 0, &data_02158f54);
  Sound_PlaySe(0x1000a);
  UnitInfoPanel_OpenForTypeOnSecondScreen(
      data_027c027c->unk_48[data_027c0284]->currentPlayer,
      gBuildMenuEntries[0].unitType);
  SidePanel_Place(SIDE_PANEL_SNAP_LEFT);
}
