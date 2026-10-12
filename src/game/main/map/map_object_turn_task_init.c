#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object_turn_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* First step of the map object turn script: starts the walk at the first object. */
void MapObjectTurnTask_Init(MapObjectTurnTask* task) {
  task->nextObject = STATE->objects;
}
