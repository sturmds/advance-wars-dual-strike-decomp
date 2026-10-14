#include "types.h"
#include "game/task.h"

s8 Task_DestroyByScript(const void* descriptor);
void MainLoop_SetFrameCallback(const void* descriptor);
extern TaskScriptCmd data_02168318[];
extern u8 MainLoop_MapFrame[];

void func_020dc71c(void) {
  Task_DestroyByScript(data_02168318);
  MainLoop_SetFrameCallback(MainLoop_MapFrame);
}
