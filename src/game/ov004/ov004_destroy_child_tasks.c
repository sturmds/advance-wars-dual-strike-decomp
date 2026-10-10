#include "types.h"
#include "game/Unk_ov004_022bba9c.h"

void Task_Destroy(struct Task *task);

void Ov004_DestroyChildTasks(Unk_ov004_022bba9c *scene) {
    int i;

    for (i = 0; i < 16; i++) {
        if (scene->childTasks[i] != NULL) {
            Task_Destroy(scene->childTasks[i]);
            scene->childTasks[i] = NULL;
        }
    }
}
