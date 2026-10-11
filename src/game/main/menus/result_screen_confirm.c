#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void Sound_PlaySe(u32 soundId);
void Task_ResumeByScript(const TaskScriptCmd *script);

extern const TaskScriptCmd data_02159ff8[];

/* Plays the confirm sound, closes the menu buttons and resumes the task
 * that runs the data_02159ff8 script. */
void ResultScreen_Confirm(void)
{
    Sound_PlaySe(0x10006);
    MenuButtons_CloseAll();
    Task_ResumeByScript(data_02159ff8);
}
