#include "types.h"
#include "game/task.h"

void Save_LoadCombatSetup(void);
void MainMenu_ReturnToPage5(void);
void func_02103714(void);
void func_02082a5c(void);

/* A guess at the task: with unk_52 set it runs three calls of unknown
 * purpose, otherwise func_02082a5c (swap overlay slot 1 for slot 3, then
 * Ov003_Init). */
void func_0208af80(Task* task) {
  if (task->unk_52 != 0) {
    Save_LoadCombatSetup();
    MainMenu_ReturnToPage5();
    func_02103714();
  } else {
    func_02082a5c();
  }
}
