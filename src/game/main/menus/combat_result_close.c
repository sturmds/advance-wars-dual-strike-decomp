#include "types.h"
#include "game/task.h"

void PlayRecords_AddCoins(u32 amount);
s16 func_02038150(void);               /* points left to hand out */
void func_02038178(void);
void Task_DestroyByScript(const TaskScriptCmd *script);
extern const TaskScriptCmd data_0216168c[];

/* Adds the points left to hand out to the coin count, runs func_02038178
 * and destroys the Combat result task (data_0216168c). */
void CombatResult_Close(void)
{
    PlayRecords_AddCoins(func_02038150());
    func_02038178();
    Task_DestroyByScript(data_0216168c);
}
