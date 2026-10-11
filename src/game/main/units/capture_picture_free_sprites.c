#include "types.h"
#include "game/capture_picture_task.h"

void func_0202244c(void *handle);

/* Destroy callback of the capture picture task: frees its three sprites. */
void CapturePicture_FreeSprites(CapturePictureTask *task)
{
    func_0202244c(task->picture);
    func_0202244c(task->unk_34);
    func_0202244c(task->unk_38);
}
