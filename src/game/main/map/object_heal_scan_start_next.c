#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/object_heal_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02167ff0[];

void* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(void* task);
void MapObject_GetKindOffset(int kind, s16* offset);

/* Tile offset lists (Map_AddTileOffset) and HP each object heals. */
#define CRYSTAL_HEAL_OFFSETS 2
#define OBELISK_HEAL_OFFSETS 4
#define OBJECT_HEAL_STEPS 2

/* Step of the object heal scan: waits while a heal task runs, then starts
 * one for the next standing Black Crystal or Black Obelisk. The obelisk's
 * tile is moved by the offset MapObject_GetKindOffset gives for its kind. Resumes
 * the script once no object is left. */
void ObjectHealScan_StartNext(ObjectHealScanTask* task) {
  MapObject* object;
  ObjectHealTask* heal;
  s16 offset[2];

  if (Task_FindByScript(data_02167ff0) != NULL) {
    return;
  }
  object = &MAP_STATE->objects[task->nextObject];
  while (object->kind != 0) {
    task->nextObject++;
    if (MapObject_IsStanding(object)) {
      switch (object->kind) {
        case MAP_OBJECT_BLACK_CRYSTAL:
          heal = (ObjectHealTask*)Task_Create(data_02167ff0, task);
          heal->tileX = object->x;
          heal->tileY = object->y;
          heal->offsetList = CRYSTAL_HEAL_OFFSETS;
          heal->objectKind = object->kind;
          heal->healSteps = OBJECT_HEAL_STEPS;
          return;
        case MAP_OBJECT_BLACK_OBELISK:
          MapObject_GetKindOffset(MAP_OBJECT_BLACK_OBELISK, offset);
          heal = (ObjectHealTask*)Task_Create(data_02167ff0, task);
          heal->tileX = object->x + offset[0];
          heal->tileY = object->y + offset[1];
          heal->offsetList = OBELISK_HEAL_OFFSETS;
          heal->objectKind = object->kind;
          heal->healSteps = OBJECT_HEAL_STEPS;
          return;
      }
    }
    object++;
  }
  Task_ResumeScript(task);
}
