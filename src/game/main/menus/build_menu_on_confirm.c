#include "types.h"
#include "game/build_menu.h"
#include "game/menu_button.h"
#include "game/map_unit.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern TaskScriptCmd data_0216594c[];

#define GAME (data_027c027c->unk_48[data_027c0284])

void Sound_PlaySe(int id);
MapUnit* Unit_FindFreeSlot(int player);
void func_020bba44(void);
MapUnit* CurrentPlayer_BuyUnit(int x, int y, int unitType);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
int MapScript_RunList10(int id, MapUnit* unit);
void func_020bbe54(void);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020f79a4(void);
void Task_ResumeByScript(void* script);

/* Confirm callback of a build menu row. A unit the player cannot afford
 * plays the refusal sound. Otherwise, if the player has a free unit slot,
 * buys the unit on tile (cursorX, cursorY), sends the action to the other
 * console when gLinkPlay is set (likely link play), runs map script
 * list 10 with id 14 for the new unit and resumes the build menu task. */
void BuildMenu_OnConfirm(MenuButton* button) {
  u8 id;
  MapUnit* unit;

  id = button->id;
  if (gBuildMenuEntries[id].state == BUILD_ENTRY_TOO_EXPENSIVE) {
    Sound_PlaySe(0x10008);
    return;
  }
  Sound_PlaySe(0x1000a);
  if (Unit_FindFreeSlot(GAME->currentPlayer) == NULL) {
    func_020bba44();
    return;
  }
  unit = CurrentPlayer_BuyUnit(GAME->cursorX, GAME->cursorY,
                               gBuildMenuEntries[id].unitType);
  GAME->actingUnitDestX = GAME->cursorX;
  GAME->actingUnitDestY = GAME->cursorY;
  if (gLinkPlay != 0) {
    UnitAction_Send(1, gBuildMenuEntries[id].unitType, 0, 0);
  }
  MapScript_RunList10(14, unit);
  func_020bbe54();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  if (data_027c027c->unk_50[data_027c0284]->second != NULL) {
    data_027c027c->unk_50[data_027c0284]->second();
  }
  func_020f79a4();
  Task_ResumeByScript(data_0216594c);
}
