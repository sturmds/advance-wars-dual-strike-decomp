#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Stores the battle's map and second map in the game state. */
void State_SetMapIds(int mapId, int secondMapId)
{
    data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId = mapId;
    data_027c027c->unk_48[data_027c0284]->secondMapId = secondMapId;
}
