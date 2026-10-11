#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern TaskScriptCmd data_02167500[];

BOOL Player1Team_HasPlayerInGame(void);
void Task_SetScript(Task *task, void *script);

/* Script step of the task BattleRecordTask_Start makes. In a campaign
 * mission with no one from player 1's team left in the game, switches the
 * task to the script at data_02167500, which runs the callback. */
void BattleRecordTask_ExitIfPlayerOut(Task *task)
{
    if (data_027c027c->unk_48[data_027c0284]->gameMode != GAME_MODE_CAMPAIGN) {
        return;
    }
    if (Player1Team_HasPlayerInGame()) {
        return;
    }
    Task_SetScript(task, data_02167500);
}
