#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

void Task_ResumeByScript(const void *script);
void Sound_PlaySe(u32 soundId);
void Gfx_ClearLayerMap(int value);
void Gfx_RequestLayerCopy(int layer);

extern const TaskScriptCmd data_0215a690[];

/* Back key or button of a page that the screen script (data_0215a690) runs:
 * plays sound 0x10006, closes the back button and resumes the script. */
void ResultStats_ClosePage(void)
{
    Sound_PlaySe(0x10006);
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
    MenuButton_Close(0);
    Task_ResumeByScript(data_0215a690);
}
