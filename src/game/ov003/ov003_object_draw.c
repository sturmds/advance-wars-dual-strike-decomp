#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_object.h"

void Ov003_DrawTexturedQuad(int x, int y, int a, int b, int c, int d, int e,
                            int f, int g);

/* Draws one object at its position relative to the camera, unless it is off
 * screen. Kinds 5 and 7 use one sprite, the others another. */
void Ov003Object_Draw(Ov003Object* object) {
  int scrollX;
  int x;
  int scrollY;
  int y;

  x = object->x >> 3;
  scrollX = data_ov003_02364928->scrollX;
  if (x < scrollX - 3 || x > scrollX + 260) {
    return;
  }
  scrollY = data_ov003_02364928->scrollY;
  y = object->y >> 3;
  if (y < scrollY - 3 || y > scrollY + 196) {
    return;
  }
  if (object->kind == 5 || object->kind == 7) {
    Ov003_DrawTexturedQuad(x - scrollX - 4, y - scrollY - 4, 25, 0, 0, 0x347,
                           object->row + 10, 0, 0);
  } else {
    Ov003_DrawTexturedQuad(x - scrollX - 4, y - scrollY - 4, 25, 0, 0, 0x346,
                           object->row + 10, 0, 0);
  }
}
