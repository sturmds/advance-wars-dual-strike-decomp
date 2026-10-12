#include "types.h"
#include "game/ov003_object.h"

void Ov003Object_Travel(Ov003Object* object);
void Ov003Object_Ease(Ov003Object* object);
void Ov003Object_MoveAndHit(Ov003Object* object);

void Ov003_UpdateObjects(void) {
  Ov003Object* object = data_ov003_02369738;
  int i;

  for (i = 0; i < OV003_OBJECT_COUNT; i++, object++) {
    if (object->active != 0) {
      switch (object->kind) {
        case 5:
          Ov003Object_Ease(object);
          break;
        case 7:
          Ov003Object_Travel(object);
          break;
        default:
          Ov003Object_MoveAndHit(object);
          break;
      }
    }
  }
}
