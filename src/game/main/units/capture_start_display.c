#include "types.h"
#include "game/Unk_02080440.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_021690f8[];
extern const TaskScriptCmd data_02168df0[];

/* Starts the capture display for the property at tile (x, y): a task on
 * list 3 (script data_021690f8) that knows the tile with its new owner, and
 * under it the picture task (script data_02168df0) with the tile as it is
 * now. Both sit at the tile's screen position, kept 32 pixels from the left
 * and right edges and between y 80 and 192. Points go from oldPoints to
 * newPoints (20 completes the capture). */
void Capture_StartDisplay(int x, int y, int oldPoints, int newPoints,
                          int owner) {
  CapturePictureTask* task;
  int screenX = x * 16 - MAP_STATE->unk_1c4->scrollX + 8;
  int screenY = y * 16 - MAP_STATE->unk_1c4->scrollY + 16;

  if (screenX < 32) {
    screenX = 32;
  }
  if (screenX > 224) {
    screenX = 224;
  }
  if (screenY < 80) {
    screenY = 80;
  }
  if (screenY > 192) {
    screenY = 192;
  }
  task = (CapturePictureTask*)Task_Create(data_021690f8, TASK_LIST(3));
  task->x = screenX;
  task->y = screenY;
  task->oldPoints = oldPoints;
  task->newPoints = newPoints;
  task->owner = owner;
  task->tile = MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)) | (owner << 5);
  task = (CapturePictureTask*)Task_Create(data_02168df0, task);
  task->x = screenX;
  task->y = screenY;
  task->oldPoints = oldPoints;
  task->newPoints = newPoints;
  task->owner = owner;
  task->oldOwner = MAP_TILE_OWNER(MAP_TILE_AT(x, y));
  task->tile = MAP_TILE_AT(x, y);
}
