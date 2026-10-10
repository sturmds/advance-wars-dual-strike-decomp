#include "types.h"
#include "game/BattleCellTask.h"

void BattleCellTask_MakeSprites(BattleCellTask *task);
void BattleActor_EnterPhase4(BattleCellTask *task);

/* Runs one step (0-5) of a cell task's steps ring; other values do nothing. */
void BattleCellTask_RunStep(BattleCellTask *task, int command)
{
    switch (command) {
    case 0:
        BattleCellTask_MakeSprites(task);
        break;
    case 1:
        BattleCellTask_StartMove(task);
        break;
    case 2:
        BattleCellTask_RemakeSprites(task);
        break;
    case 3:
        BattleCellTask_MakeMatchupSprites(task);
        break;
    case 4:
        BattleActor_EnterPhase4(task);
        break;
    case 5:
        BattleCellTask_RemakeSpritesWithEffect(task);
        break;
    }
}
