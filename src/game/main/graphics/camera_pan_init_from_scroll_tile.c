#include "types.h"
#include "game/Unk_02080440.h"
#include "game/camera_pan.h"

extern Camera data_022a7d5c;

void Camera_TileToWorld(s32* out, int x, int y);

/* Sets up a 32-frame camera pan that starts at the world position of the
 * tile the view scrolls to (state->cursorX, unk_2a) and ends at the camera
 * position now. `to` is a local pointer to match. */
void CameraPan_InitFromScrollTile(CameraPanTask* pan) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
  s32* to = pan->to;

  Camera_TileToWorld(pan->from, state->cursorX, state->cursorY);
  pan->to[0] = data_022a7d5c.x;
  to[1] = data_022a7d5c.y;
  to[2] = data_022a7d5c.z;
  pan->frames = 32;
  pan->frame = 0;
}
