#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern const TaskScriptCmd data_021668e8[];

void Scene_SetKind(int value);
int SceneMode_Request(int value);

/* Sets scene fields 0x00 to 0 and 0x0c to 5 (through Scene_SetKind and
 * SceneMode_Request). On the first screen it also starts the data_021668e8 child
 * task of parent. */
void func_020f38ac(Task* parent) {
  if (data_027c027c == data_027c0288.unk_00) {
    Scene_SetKind(0);
    SceneMode_Request(5);
    Task_CreateChild(data_021668e8, parent);
  } else {
    Scene_SetKind(0);
    SceneMode_Request(5);
  }
}
