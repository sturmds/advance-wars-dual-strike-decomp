#include "types.h"
#include "game/ov003_object.h"

s32 HwMod32(s32 numerator, s32 denominator);
void Ov003_StepToward(int x1, int y1, int x2, int y2, int length, int* outX,
                      int* outY);
int Ov003Object_HitAt(Ov003Object* object, int x, int y);

/* Default kind: moves the object along (offsetX, offsetY) by active units per
 * frame, up to 128 units per point of its kind's range. Before it moves it
 * tests the points between where it is and where it goes (x and y over 8)
 * with Ov003Object_HitAt, and stops testing at the first hit. On a diagonal
 * the minor axis offset is (i * major) mod minor, not a straight line. On the frame it
 * reaches the range it clears active and does not move. */
void Ov003Object_MoveAndHit(Ov003Object* object) {
  int deltaX;
  int deltaY;
  int stepX;
  int stepY;
  int i;
  int range = data_ov003_02364d8c[object->kind].range << 7;
  int step = object->active;
  int travelled = object->travelled;

  if (travelled + step > range) {
    object->active = 0;
    step = range - travelled;
  }
  Ov003_StepToward(0, 0, object->offsetX, object->offsetY, step, &deltaX,
                   &deltaY);
  stepX = deltaX >> 3;
  stepY = deltaY >> 3;
  if (stepX == 0) {
    if (stepY > 0) {
      for (i = 0; i <= stepY; i++) {
        if (Ov003Object_HitAt(object, object->x >> 3, i + (object->y >> 3)) ==
            1) {
          break;
        }
      }
    } else {
      for (i = 0; i >= stepY; i--) {
        if (Ov003Object_HitAt(object, object->x >> 3, i + (object->y >> 3)) ==
            1) {
          break;
        }
      }
    }
  } else if (stepY == 0) {
    if (stepX > 0) {
      for (i = 0; i <= stepX; i++) {
        if (Ov003Object_HitAt(object, i + (object->x >> 3), object->y >> 3) ==
            1) {
          break;
        }
      }
    } else {
      for (i = 0; i >= stepX; i--) {
        if (Ov003Object_HitAt(object, i + (object->x >> 3), object->y >> 3) ==
            1) {
          break;
        }
      }
    }
  } else if ((stepX < 0 ? -stepX : stepX) > (stepY < 0 ? -stepY : stepY)) {
    if (stepX > 0) {
      for (i = 0; i <= stepX; i++) {
        if (Ov003Object_HitAt(object, i + (object->x >> 3),
                              HwMod32(i * stepX, stepY) + (object->y >> 3)) ==
            1) {
          break;
        }
      }
    } else {
      for (i = 0; i >= stepX; i--) {
        if (Ov003Object_HitAt(object, i + (object->x >> 3),
                              HwMod32(i * stepX, stepY) + (object->y >> 3)) ==
            1) {
          break;
        }
      }
    }
  } else {
    if (stepY > 0) {
      for (i = 0; i <= stepY; i++) {
        if (Ov003Object_HitAt(object,
                              HwMod32(i * stepY, stepX) + (object->x >> 3),
                              i + (object->y >> 3)) == 1) {
          break;
        }
      }
    } else {
      for (i = 0; i >= stepY; i--) {
        if (Ov003Object_HitAt(object,
                              HwMod32(i * stepY, stepX) + (object->x >> 3),
                              i + (object->y >> 3)) == 1) {
          break;
        }
      }
    }
  }
  if (object->active == 0) {
    return;
  }
  object->x += deltaX;
  object->y += deltaY;
  object->travelled += object->active;
}
