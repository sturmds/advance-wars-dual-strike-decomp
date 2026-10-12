#include "types.h"
#include "game/ov003_object.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Ov003Object_Impact(Ov003Object* object);

/* Kind 5: moves the object along the vector (offsetX, offsetY) with easing.
 * The trip takes (range * 128) / active frames; on the last frame it clears
 * active and calls Ov003Object_Impact. */
void Ov003Object_Ease(Ov003Object* object) {
  int duration =
      HwDiv32(data_ov003_02364d8c[object->kind].range << 7, object->active);

  object->x +=
      Math_Ease(0, 0, object->offsetX * 8, object->travelled + 1, duration) -
      Math_Ease(0, 0, object->offsetX * 8, object->travelled, duration);
  object->y +=
      Math_Ease(0, 0, object->offsetY * 8, object->travelled + 1, duration) -
      Math_Ease(0, 0, object->offsetY * 8, object->travelled, duration);
  object->travelled++;
  if (object->travelled == duration) {
    object->active = 0;
    Ov003Object_Impact(object);
  }
}
