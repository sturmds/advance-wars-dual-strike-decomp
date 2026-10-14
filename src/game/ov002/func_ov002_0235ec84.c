#include "types.h"
#include "game/task.h"

void BattleCellSpriteTask_Init(void);
void BattleFighterTask_RespawnSprite(void);
void BattleFighterTask_Setup(void);
void BattleFighterTask_SpawnStep(void);
void BattleFighterTask_SpawnStepOrFinish(void);
void Battle_UpdateCueSpawner(void);

TaskScriptCmd data_ov002_02369744[4] = {
#include "data/ov002/data_ov002_02369744.inc.c"
};
TaskScriptCmd data_ov002_02369724[4] = {
#include "data/ov002/data_ov002_02369724.inc.c"
};
TaskScriptCmd data_ov002_02369764[4] = {
#include "data/ov002/data_ov002_02369764.inc.c"
};
void Task_DestroyByScript(const TaskScriptCmd* script);

void func_ov002_0235ec84(void) {
  Task_DestroyByScript(data_ov002_02369724);
  Task_DestroyByScript(data_ov002_02369744);
  Task_DestroyByScript(data_ov002_02369764);
}
