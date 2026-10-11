#include "types.h"
#include "game/task.h"

void func_02018ea0(void);
void func_020305e4(void);
void func_02103714(void);
void func_02082a5c(void);

/* A guess at the task: with unk_52 set it runs three calls of unknown
 * purpose, otherwise func_02082a5c (swap overlay slot 1 for slot 3, then
 * Ov003_Init). */
void func_0208af80(Task *task)
{
    if (task->unk_52 != 0) {
        func_02018ea0();
        func_020305e4();
        func_02103714();
    } else {
        func_02082a5c();
    }
}
