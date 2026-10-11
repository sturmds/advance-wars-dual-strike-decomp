#include "types.h"
#include "game/task.h"
#include "game/play_records.h"
#include "game/Unk_022a7b80.h"

BOOL Player1Team_HasPlayerInGame(void);
void GameFlag_Set(int id, int value);
int Survival_GetMapCount(int mode);
void func_020eae84(void);

extern Unk_022a7b80 data_022a7b80;
extern const TaskScriptCmd data_02167294[];
extern const TaskScriptCmd data_02167314[];

#define RECORD_MAX 999999

/* Runs when a map of a run ends (mode variant in data_022a7b80.unk_07).
 * A win (player 1's team still in) counts in survivalMapsWon. A win on the
 * run's last map in modes 0-2 sets game flag 44, 45 or 46, runs
 * func_020eae84, counts in survivalRunsCleared and starts data_02167314.
 * Any other win starts data_02167294. A loss runs func_020eae84 in modes
 * 3-5 and starts data_02167314. */
void Survival_FinishMap(void)
{
    BOOL won = Player1Team_HasPlayerInGame();
    BOOL lastMap = data_022a7b80.unk_06 == Survival_GetMapCount(data_022a7b80.unk_07);

    if (won) {
        switch (data_022a7b80.unk_07) {
        case 0:
        case 3:
            if (gPlayRecords.survivalMapsWon[0] < RECORD_MAX) {
                gPlayRecords.survivalMapsWon[0]++;
            }
            break;
        case 2:
        case 5:
            if (gPlayRecords.survivalMapsWon[1] < RECORD_MAX) {
                gPlayRecords.survivalMapsWon[1]++;
            }
            break;
        case 1:
        case 4:
            if (gPlayRecords.survivalMapsWon[2] < RECORD_MAX) {
                gPlayRecords.survivalMapsWon[2]++;
            }
            break;
        }
        if (lastMap) {
            switch (data_022a7b80.unk_07) {
            case 0:
                GameFlag_Set(44, 1);
                break;
            case 2:
                GameFlag_Set(45, 1);
                break;
            case 1:
                GameFlag_Set(46, 1);
                break;
            default:
                Task_Create(data_02167294, TASK_LIST(3));
                return;
            }
            func_020eae84();
            switch (data_022a7b80.unk_07) {
            case 0:
                if (gPlayRecords.survivalRunsCleared[0] < RECORD_MAX) {
                    gPlayRecords.survivalRunsCleared[0]++;
                }
                break;
            case 2:
                if (gPlayRecords.survivalRunsCleared[1] < RECORD_MAX) {
                    gPlayRecords.survivalRunsCleared[1]++;
                }
                break;
            case 1:
                if (gPlayRecords.survivalRunsCleared[2] < RECORD_MAX) {
                    gPlayRecords.survivalRunsCleared[2]++;
                }
                break;
            }
            Task_Create(data_02167314, TASK_LIST(3));
        } else {
            Task_Create(data_02167294, TASK_LIST(3));
        }
    } else {
        switch (data_022a7b80.unk_07) {
        case 0:
        case 1:
        case 2:
            break;
        default:
            func_020eae84();
            break;
        }
        Task_Create(data_02167314, TASK_LIST(3));
    }
}
