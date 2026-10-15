#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

void MapObjects_ClearHead(void);

/* Empties the object list: ends it at the first entry, then wipes the first 16 entries (MapObjects_ClearHead). */
void MapObjects_Clear(void) {
  data_027c027c->unk_48[data_027c0284]->objects[0].kind = 0;
  MapObjects_ClearHead();
}
