#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Map_InitBattle(void);
void Map_SetupPlayerCos(void);
void Save_LoadProgress(void);
void Save_LoadSurvivalState(void);
void SetupMapRules(void);
void Survival_EnterGameMode(void);
void Survival_SavePlayerCos(void);
void Survival_StartRun(void);
extern u8 data_0216d6fc[];
void func_0204184c(void);
void func_02045890(void);
void func_0205cd80(void);
void func_020b983c(void);
void func_020e8970(void);

static TaskScriptCmd data_0216718c[13] = {
#include "data/main/data_0216718c.inc.c"
};
TaskScriptCmd gSurvivalNewRunScript[20] = {
#include "data/main/gSurvivalNewRunScript.inc.c"
};

void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

/* Sets gameMode 1 on both screens, loads the Force Rank sets (mode 1) and
 * starts the data_0216718c task. */
void func_0210322c(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_WAR_ROOM;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_WAR_ROOM;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  ForceRank_LoadSets(1);
  Task_Create(data_0216718c, TASK_LIST(3));
}
