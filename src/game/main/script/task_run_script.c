#include "types.h"
#include "game/task.h"

void func_020235d0(void);
void Task_SkipCommand(void);
void TaskCmd_CallAndContinue(void);
void TaskCmd_SetUpdate(void);
void func_020234b0(void);
void TaskCmd_StartTaskUnderThis(void);
void TaskCmd_StartChildAndWait(void);
void TaskCmd_StartTaskInList(void);
void TaskCmd_WaitForScriptEnd(void);
void func_020233c0(void);
void func_02023394(void);
void func_02023380(void);
void func_02023338(void);
void func_0202335c(void);
void TaskCmd_WaitFrames(void);
void TaskCmd_SetGroup(void);
void func_020231c8(void);
void TaskCmd_ExitIfScriptRunning(void);
void func_020230bc(void);
void func_020231d0(void);
void TaskCmd_CallWhileBusy(void);
void func_020230dc(void);
void TaskCmd_CallWithTask(void);
void TaskCmd_DestroyOtherWithScript(void);
void TaskCmd_CallWithArg(void);
void TaskCmd_StartEventScript(void);
void TaskCmd_WaitForEventScriptEnd(void);
void func_02023090(void);
void func_02023064(void);
void func_02023034(void);
void func_02022fa4(void);
void func_02022fd4(void);
void func_02023004(void);
void func_02022f78(void);
void func_02022f4c(void);
void func_02022db0(void);
void func_02022d84(void);
void func_02022d54(void);
void func_02022d24(void);
void func_02022cf4(void);
void func_02022cc4(void);
void func_02022c98(void);
void func_02022c6c(void);
void func_02022c3c(void);
void func_02022c0c(void);
void func_02022b58(void);
void TaskCmd_StartTaskOnBothScreens(void);
void func_020229e8(void);
void TaskCmd_StartChildOnBothScreens(void);
void func_02022f20(void);
void func_02022ef4(void);
void func_02022ec4(void);
void func_02022e94(void);
void func_02022e68(void);
void func_02022e3c(void);
void func_02022e0c(void);
void func_02022ddc(void);
void TaskCmd_JumpToLabelIfCheck(void);
void TaskCmd_JumpToLabelUnlessCheck(void);
void func_0202326c(void);

static TaskCommandFunc gTaskCommands[] = {
    (TaskCommandFunc)&func_020235d0,
    (TaskCommandFunc)&Task_SkipCommand,
    (TaskCommandFunc)&TaskCmd_CallAndContinue,
    (TaskCommandFunc)&TaskCmd_SetUpdate,
    (TaskCommandFunc)&func_020234b0,
    (TaskCommandFunc)&TaskCmd_StartTaskUnderThis,
    (TaskCommandFunc)&TaskCmd_StartChildAndWait,
    (TaskCommandFunc)&TaskCmd_StartTaskInList,
    (TaskCommandFunc)&TaskCmd_WaitForScriptEnd,
    (TaskCommandFunc)&func_020233c0,
    (TaskCommandFunc)&func_02023394,
    (TaskCommandFunc)&func_02023380,
    (TaskCommandFunc)&func_02023338,
    (TaskCommandFunc)&func_0202335c,
    (TaskCommandFunc)&TaskCmd_WaitFrames,
    (TaskCommandFunc)&TaskCmd_SetGroup,
    (TaskCommandFunc)&func_020231c8,
    (TaskCommandFunc)&TaskCmd_ExitIfScriptRunning,
    (TaskCommandFunc)&func_020230bc,
    (TaskCommandFunc)&func_020231d0,
    (TaskCommandFunc)&TaskCmd_CallWhileBusy,
    (TaskCommandFunc)&func_020230dc,
    (TaskCommandFunc)&TaskCmd_CallWithTask,
    (TaskCommandFunc)&TaskCmd_DestroyOtherWithScript,
    (TaskCommandFunc)&TaskCmd_CallWithArg,
    (TaskCommandFunc)&TaskCmd_StartEventScript,
    (TaskCommandFunc)&TaskCmd_WaitForEventScriptEnd,
    (TaskCommandFunc)&func_02023090,
    (TaskCommandFunc)&func_02023064,
    (TaskCommandFunc)&func_02023034,
    (TaskCommandFunc)&func_02022fa4,
    (TaskCommandFunc)&func_02022fd4,
    (TaskCommandFunc)&func_02023004,
    (TaskCommandFunc)&func_02022f78,
    (TaskCommandFunc)&func_02022f4c,
    (TaskCommandFunc)&func_02022db0,
    (TaskCommandFunc)&func_02022d84,
    (TaskCommandFunc)&func_02022d54,
    (TaskCommandFunc)&func_02022d24,
    (TaskCommandFunc)&func_02022cf4,
    (TaskCommandFunc)&func_02022cc4,
    (TaskCommandFunc)&func_02022c98,
    (TaskCommandFunc)&func_02022c6c,
    (TaskCommandFunc)&func_02022c3c,
    (TaskCommandFunc)&func_02022c0c,
    (TaskCommandFunc)&func_02022b58,
    (TaskCommandFunc)&TaskCmd_StartTaskOnBothScreens,
    (TaskCommandFunc)&func_020229e8,
    (TaskCommandFunc)&TaskCmd_StartChildOnBothScreens,
    (TaskCommandFunc)&func_02022f20,
    (TaskCommandFunc)&func_02022ef4,
    (TaskCommandFunc)&func_02022ec4,
    (TaskCommandFunc)&func_02022e94,
    (TaskCommandFunc)&func_02022e68,
    (TaskCommandFunc)&func_02022e3c,
    (TaskCommandFunc)&func_02022e0c,
    (TaskCommandFunc)&func_02022ddc,
    (TaskCommandFunc)&TaskCmd_JumpToLabelIfCheck,
    (TaskCommandFunc)&TaskCmd_JumpToLabelUnlessCheck,
    (TaskCommandFunc)&func_0202326c,
    (TaskCommandFunc)&func_02023380,
    (TaskCommandFunc)&func_020235d0};

/* Runs the task's script commands until a handler returns 0, the task
 * ends or gets an update function. Task_Create calls it once on a new
 * task; Task_RunTree calls it each frame for tasks without an update.
 * Needed to match: the first command is written out before the loop. */
void Task_RunScript(Task* task) {
  if (task->script == NULL || task->childCount != 0 || task->update != NULL) {
    return;
  }
  if (!gTaskCommands[task->pc->op](task)) {
    return;
  }
  do {
    if (task->script == NULL || task->childCount != 0 || task->update != NULL) {
      return;
    }
  } while (gTaskCommands[task->pc->op](task));
}
