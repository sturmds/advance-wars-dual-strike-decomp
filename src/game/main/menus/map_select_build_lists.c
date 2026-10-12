#include "types.h"
#include "game/menu_state.h"

int MapSelect_FillList(MapList *list, int kind, int playerCount);

#define KIND_SKIPPED 7

/* Builds the lists of list kinds 1-8 except 7 (the Survival maps,
 * MAP_CASE_SURVIVAL), skipping empty ones. playerCount 0: any map size, else
 * only maps with that many players. */
void MapSelect_BuildLists(MapListSet *set, int playerCount)
{
    int kind;

    set->count = 0;
    for (kind = 1; kind < 9; kind++) {
        if (kind == KIND_SKIPPED) {
            continue;
        }
        if (MapSelect_FillList(&set->lists[set->count], kind, playerCount) == 1) {
            set->count++;
        }
    }
}
