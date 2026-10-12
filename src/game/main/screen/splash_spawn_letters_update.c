#include "types.h"
#include "game/task.h"
#include "game/splash_task.h"

void Task_ResumeScript(void* task);

extern const TaskScriptCmd data_02169cac[];

/* On frame 10 starts the four letter tasks, numbered 0 to 3. On frame 50
 * resumes the script. */
void Splash_SpawnLettersUpdate(SplashTask* task) {
  SplashTask* letter;

  /* the four starts are written out; a loop does not match */
  switch (task->step) {
    case 10:
      letter = (SplashTask*)Task_Create(data_02169cac, TASK_LIST(3));
      letter->letterIndex = task->lettersMade++;
      letter = (SplashTask*)Task_Create(data_02169cac, TASK_LIST(3));
      letter->letterIndex = task->lettersMade++;
      letter = (SplashTask*)Task_Create(data_02169cac, TASK_LIST(3));
      letter->letterIndex = task->lettersMade++;
      letter = (SplashTask*)Task_Create(data_02169cac, TASK_LIST(3));
      letter->letterIndex = task->lettersMade++;
      break;
    case 50:
      Task_ResumeScript(task);
      break;
  }
  task->step++;
}
