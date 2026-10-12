#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160ac0.h"
#include "game/Unk_02160abc.h"

extern Unk_027c0288 data_027c0288;

void func_020014f8(void);
void Screen_SetCurrentBoth(void* state);
void Screen_SetBothNibbles(int value);
void Screens_HideWithWindows(void);
void Save_LoadCombatSetup(void);
void CombatSetup_StartKeepingSettings(void);
void MainMenu_ReturnToPage5(void);
void func_02103714(void);
void func_0208af34(void);

/* Script step of gOv003ExitTaskScript: picks what follows the battle. A nonzero
 * data_02160ac0->unk_7c starts the Combat setup task with its settings kept. Otherwise the
 * saved Combat setup is loaded and the main menu task starts, unless
 * data_02160abc->unk_0a is 1, which starts the task of func_0208af34. */
void Ov003ExitTask_Run(void) {
  func_020014f8();
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  if (data_02160ac0->unk_7c != 0) {
    Screen_SetBothNibbles(1);
    Screens_HideWithWindows();
    CombatSetup_StartKeepingSettings();
    return;
  }
  if (data_02160abc->unk_0a != 1) {
    Save_LoadCombatSetup();
    Screen_SetBothNibbles(1);
    Screens_HideWithWindows();
    MainMenu_ReturnToPage5();
    func_02103714();
  } else {
    func_0208af34();
  }
}
