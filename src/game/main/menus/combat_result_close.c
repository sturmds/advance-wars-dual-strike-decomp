#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

void PlayRecords_AddCoins(u32 amount);
void ResultPoint_Close(void);
void Task_DestroyByScript(const TaskScriptCmd* script);
extern const TaskScriptCmd data_0216168c[];

/* Adds the result point tally to the coin count, runs ResultPoint_Close
 * and destroys the Combat result task (data_0216168c). */
void CombatResult_Close(void) {
  PlayRecords_AddCoins(ResultPoint_GetTally());
  ResultPoint_Close();
  Task_DestroyByScript(data_0216168c);
}
