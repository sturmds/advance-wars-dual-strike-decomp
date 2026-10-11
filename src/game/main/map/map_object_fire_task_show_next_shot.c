#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/map_object_turn_task.h"
#include "game/map_unit.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/task.h"
#include "game/unit_target_entry.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern UnitTargetEntry *data_022a6dcc;
extern void *data_02167e98[4];

MapObject *Map_FindObjectCovering(int x, int y);
void TileListTask_StartChild(int tileX, int tileY, void *list, Task *parent);
void CannonShotTask_Start(int cannonX, int cannonY, int targetX, int targetY, int kind, int facing, MapObjectFireTask *parent);
int Task_JumpToLabel(MapObjectFireTask *task, int label);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the map object fire script. The AI target list starts with object
 * entries (unit slot 0, value the object kind, x and y its tile),
 * then the units hit. While nextEntry is still on an object entry, starts
 * that object's shot under the task and jumps back to label 0: the eruption
 * tiles for a volcano, a cannon shot at the first unit in the list for a
 * Black Cannon or Minicannon. A cannon shot at a human player's unit counts
 * in gPlayRecords.cannonHitsTaken. */
void MapObjectFireTask_ShowNextShot(MapObjectFireTask *task)
{
    UnitTargetEntry *list = data_022a6dcc;
    UnitTargetEntry *entry;
    MapUnit *unit;
    MapObject *object;
    int i;

    i = 0;
    while (list[i].unitSlot == 0) {
        i++;
    }
    unit = &STATE->units[list[i].unitSlot];
    if (list[task->nextEntry].unitSlot != 0) {
        return;
    }
    entry = &list[task->nextEntry];
    switch (entry->value) {
    case MAP_OBJECT_VOLCANO:
        object = Map_FindObjectCovering(entry->x, entry->y);
        TileListTask_StartChild(list[task->nextEntry].x, list[task->nextEntry].y,
                                data_02167e98[object->variant], (Task *)task);
        break;
    case MAP_OBJECT_BLACK_CANNON:
        object = Map_FindObjectCovering(entry->x, entry->y);
        CannonShotTask_Start(list[task->nextEntry].x, list[task->nextEntry].y, unit->x, unit->y,
                      MAP_OBJECT_BLACK_CANNON, object->variant, task);
        if (STATE->players[MAP_UNIT_OWNER(STATE, unit)].controller == PLAYER_CONTROLLER_HUMAN) {
            if (gPlayRecords.cannonHitsTaken < 999999) {
                gPlayRecords.cannonHitsTaken++;
            }
        }
        break;
    case MAP_OBJECT_MINICANNON:
        object = Map_FindObjectCovering(entry->x, entry->y);
        CannonShotTask_Start(list[task->nextEntry].x, list[task->nextEntry].y, unit->x, unit->y,
                      MAP_OBJECT_MINICANNON, object->variant, task);
        if (STATE->players[MAP_UNIT_OWNER(STATE, unit)].controller == PLAYER_CONTROLLER_HUMAN) {
            if (gPlayRecords.cannonHitsTaken < 999999) {
                gPlayRecords.cannonHitsTaken++;
            }
        }
        break;
    }
    task->nextEntry++;
    Task_JumpToLabel(task, 0);
}
