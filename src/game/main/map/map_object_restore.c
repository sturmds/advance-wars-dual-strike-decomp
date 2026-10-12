#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);

/* Copies a saved object into slot `index` of the map's object list. */
void MapObject_Restore(MapObject* object, int index) {
  /* unitBits 2 is not 16 or 32, so this copies bytes. */
  Mem_CopyUnits(object, &data_027c027c->unk_48[data_027c0284]->objects[index],
                sizeof(MapObject), 2);
}
