#include "types.h"
#include "game/ov003_object.h"

int Math_Distance(int x1, int y1, int x2, int y2);
void Ov003_StepToward(int x1, int y1, int x2, int y2, int length, int* outX,
                      int* outY);
void Ov003Object_Impact(Ov003Object* object);

/* Kind 7: moves the object along the vector (offsetX, offsetY) by active
 * units per frame. On the frame it would pass the end of the vector it clears
 * active, moves nothing and calls Ov003Object_Impact. */
void Ov003Object_Travel(Ov003Object* object) {
  int deltaX;
  int deltaY;
  int step;
  int travelled;
  int distance = Math_Distance(object->offsetX * 8, object->offsetY * 8, 0, 0);

  step = object->active;
  travelled = object->travelled;
  if (travelled + step > distance) {
    object->active = 0;
    step = distance - travelled;
  }
  Ov003_StepToward(0, 0, object->offsetX, object->offsetY, step, &deltaX,
                   &deltaY);
  if (object->active != 0) {
    object->x += deltaX;
    object->y += deltaY;
    object->travelled += object->active;
  } else {
    Ov003Object_Impact(object);
  }
}
