#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern TaskScriptCmd data_0216594c[];

void Sound_PlaySe(int id);
void func_020bbe54(void);
void UnitInfoPanel_Close(void);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_020f79a4(void);
void Task_ResumeByScript(void* script);

/* Cancel callback of a build menu row: closes the menu without buying (the
 * tail is the same as BuildMenu_OnConfirm's) and resumes the build menu task. */
void BuildMenu_OnCancel(MenuButton* button) {
  Sound_PlaySe(0x10009);
  func_020bbe54();
  UnitInfoPanel_Close();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  if (data_027c027c->unk_50[data_027c0284]->second != NULL) {
    data_027c027c->unk_50[data_027c0284]->second();
  }
  func_020f79a4();
  Task_ResumeByScript(data_0216594c);
}
