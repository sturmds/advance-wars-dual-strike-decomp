#include "types.h"
#include "game/map_object.h"

MapObject* MapObject_Append(int x, int y, int width, int height, int kind,
                            int hp);

/* Appends an object to the map's object list and fills in its state fields. */
void Map_AddObject(int x, int y, int width, int height, int kind, int hp,
                   int state, int startState, int variant, int unk07) {
  MapObject* object = MapObject_Append(x, y, width, height, kind, hp);

  object->unk_05 = state;
  object->variant = variant;
  object->startState = startState;
  object->state = state;
  object->unk_07 = unk07;
}
