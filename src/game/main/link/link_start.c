#include "types.h"
#include "game/task.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;
extern const TaskScriptCmd data_0215fea0[];
extern const TaskScriptCmd data_0215feb8[];

void func_0207bdbc(void);
void func_0207a5b0(void);

/* Enters link mode: sets up, creates the two link tasks and sets the mode flag. */
void Link_Start(void)
{
    func_0207bdbc();
    func_0207a5b0();
    Task_Create(data_0215fea0, NULL);
    Task_Create(data_0215feb8, NULL);
    data_0215fe8c->unk_1c = 1;
}
