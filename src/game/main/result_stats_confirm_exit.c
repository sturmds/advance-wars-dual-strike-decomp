#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

void Task_ResumeByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void ResultStats_StartExit(void);

extern const TaskScriptCmd data_0215a690[];

/* A or Start: leaves the screen. Resumes the screen script, then starts the exit script. */
void ResultStats_ConfirmExit(void) {
  Sound_PlaySe(0x10006);
  MenuButton_Close(0);
  MenuButtons_DisableTouch();
  Task_ResumeByScript(data_0215a690);
  ResultStats_StartExit();
}
