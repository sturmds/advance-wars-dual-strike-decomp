#include "types.h"
#include "game/BattleCellTask.h"

void func_ov002_0235c8cc(BattleCellTask *task);
void func_ov002_0235b4f8(BattleCellTask *task);
void BattleActor_EnterPhase4(BattleCellTask *task);

/* Runs one step (0-5) of a cell task's steps ring; other values do nothing. */
void BattleCellTask_RunStep(BattleCellTask *task, int command)
{
    switch (command) {
    case 0:
        func_ov002_0235c8cc(task);
        break;
    case 1:
        BattleCellTask_StartMove(task);
        break;
    case 2:
        BattleCellTask_RemakeSprites(task);
        break;
    case 3:
        func_ov002_0235b4f8(task);
        break;
    case 4:
        BattleActor_EnterPhase4(task);
        break;
    case 5:
        BattleCellTask_RemakeSpritesWithEffect(task);
        break;
    }
}
