#include "types.h"
#include "game/task.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern TaskScriptCmd data_ov002_0236961c[];
extern TaskScriptCmd data_ov002_0236963c[];
extern TaskScriptCmd data_ov002_023604c4[];

s8 Task_DestroyByScript(const void* script);
void func_ov002_02356410(void);
void func_ov002_0235ec84(void);
void func_ov002_0235cd50(void);
void func_ov002_0235cd3c(void);
void func_ov002_02353218(void);
void Battle_ClearMotions(void);
void Battle_ClearCounterTasks(void);

/* Destroys the battle screen tasks, clears the BattleEntry words and runs the rest of the reset. */
void Battle_ResetScreen(void) {
  int i;
  int j;

  Task_DestroyByScript(data_ov002_0236961c);
  Task_DestroyByScript(data_ov002_0236963c);
  Task_DestroyByScript(data_ov002_023604c4);
  func_ov002_02356410();
  func_ov002_0235ec84();
  func_ov002_0235cd50();
  func_ov002_0235cd3c();
  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    for (j = 0; j < BATTLE_CELL_COUNT; j++) {
      gMenuState.battle.unk_310[i][j].unk_00 = 0;
    }
  }
  func_ov002_02353218();
  Battle_ClearMotions();
  Battle_ClearCounterTasks();
}
