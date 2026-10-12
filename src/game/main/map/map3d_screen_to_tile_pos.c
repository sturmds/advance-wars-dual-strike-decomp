#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern MtxFx43 data_022a7d70; /* Camera.mtx43 of the camera at data_022a7d5c */

void MTX_Inverse43(const MtxFx43* src, MtxFx43* dst);
fx32 FX_Div(fx32 numer, fx32 denom); /* FX_Div */
void VEC_MultAdd(fx32 a, const VecFx32* v1, const VecFx32* v2, VecFx32* dst);

#define FX_MUL(a, b) ((fx32)(((long long)(a) * (b) + 0x800) >> 12))

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Turns a screen point (x, y) on the 3D map screen into a map position: casts
 * a ray from the camera through the point, finds where it meets the ground
 * plane (z = 0), and snaps it to the tile it lands on, clamped to the map.
 * Tiles are 0x200 apart in x and in -y; out->z is 0. */
void Map3d_ScreenToTilePos(int x, int y, VecFx32* out) {
  /* declaration order needed to match */
  int tileX;
  int tileY;
  fx32 t;
  fx32 scale;
  fx32 sx;
  const MtxFx43* cam;
  VecFx32 right;
  fx32 sy;
  VecFx32 up;
  VecFx32 back;
  fx32 camX;
  fx32 camY;
  fx32 camZ;
  VecFx32 dir;
  MtxFx43 inv;

  /* needed to match: through an integer, so that mwcc keeps the address in a register */
  cam = (const MtxFx43*)(u32)&data_022a7d70;
  sx = (x - 128) << 12;
  sy = (96 - y) << 12;
  scale = FX_Div(0x60000, FX_Div(0x800, 0xddb));
  MTX_Inverse43(cam, &inv);
  camX = inv.m[3][0];
  camY = inv.m[3][1];
  camZ = inv.m[3][2];
  right.x = cam->m[0][0];
  right.y = cam->m[1][0];
  right.z = cam->m[2][0];
  back.x = -cam->m[0][2];
  back.y = -cam->m[1][2];
  back.z = -cam->m[2][2];
  up.x = cam->m[0][1];
  up.y = cam->m[1][1];
  up.z = cam->m[2][1];
  sx = FX_Div(sx, scale);
  sy = FX_Div(sy, scale);
  VEC_MultAdd(sx, &right, &back, &dir);
  VEC_MultAdd(sy, &up, &dir, &dir);
  t = FX_Div(camZ, dir.z);
  out->x = camX - FX_MUL(t, dir.x);
  out->y = camY - FX_MUL(t, dir.y);
  out->z = 0;

  tileX = out->x >> 9;
  tileY = -out->y >> 9;
  if (tileX < 0) {
    tileX = 0;
  }
  if (tileX >= VIEW->mapWidth) {
    tileX = VIEW->mapWidth - 1;
  }
  if (tileY < 0) {
    tileY = 0;
  }
  if (tileY >= VIEW->mapHeight) {
    tileY = VIEW->mapHeight - 1;
  }
  out->x = tileX << 9;
  out->y = -(tileY << 9);
}
