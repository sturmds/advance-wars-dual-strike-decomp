#include "types.h"
#include "game/capture_picture_task.h"

void SpriteAnim_Free(void *handle);

/* Destroy callback of the capture picture task: frees its three sprites. */
void CapturePicture_FreeSprites(CapturePictureTask *task)
{
    SpriteAnim_Free(task->textSprite);
    SpriteAnim_Free(task->armySprite);
    SpriteAnim_Free(task->propertySprite);
}
