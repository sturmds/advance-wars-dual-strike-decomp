#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"
#include "game/task.h"

void Sound_PlaySe(u32 soundId);
void func_0208ac7c(void);
void func_020b9890(void);
void Task_ResumeByScript(void* script);
extern TaskScriptCmd data_02161510[];

#define SE_CONFIRM 0x10006

/* A on a Combat mode menu slot: stores the selected slot in
 * data_02160ac0->co, flags the current player like
 * CombatModeMenu_FinishShop does, and resumes the menu script
 * (data_02161510), the counterpart of func_0208a128. */
void CombatModeMenu_PickSlot(void) {
  Sound_PlaySe(SE_CONFIRM);
  MenuButtons_RemoveSelectable();
  func_0208ac7c();
  data_02160ac0->co = Menu_GetSelectedId();
  func_020b9890();
  Task_ResumeByScript(data_02161510);
}
