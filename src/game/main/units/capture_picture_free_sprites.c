#include "types.h"
#include "game/capture_picture_task.h"

void func_0202244c(void *handle);

/* Destroy callback of the capture picture task: frees its three sprites. */
void CapturePicture_FreeSprites(CapturePictureTask *task)
{
    func_0202244c(task->textSprite);
    func_0202244c(task->armySprite);
    func_0202244c(task->propertySprite);
}
