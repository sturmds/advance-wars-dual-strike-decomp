#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/camera_pan.h"
#include "game/Unk_02088d04.h"

extern void* data_027c027c;
extern Unk_027c0288 data_027c0288;
extern Camera data_022a7d5c;
extern MtxFx44 data_022a7cac;

int MainLoop_GetFrameCount(void);
void ShimmerLevels_Update(int frame);
void Gfx3d_SetViewport(const void* viewport);
void func_0200c5d8(Camera* camera);
void Camera_UpdateViewMatrix(Camera* camera);
void Camera_BuildMatrices(Camera* camera, MtxFx44* mtx);
void func_0210b5b8(MtxFx44* mtx);
void Matrix_Expand43Into44(MtxFx43* src, MtxFx44* dest);

#define GEOM_TARGET_PROJ 0
#define GEOM_TARGET_POS_AND_DIR 2

/* Starts the 3D map frame for the screen data_027c0288.unk_00 shows: updates
 * the camera, loads data_022a7cac and then the camera's view matrix into the
 * projection matrix, and resets the position matrix. */
void Map3d_BeginFrame(void) {
  MtxFx44 view;

  if (data_027c027c != data_027c0288.unk_00) {
    return;
  }
  ShimmerLevels_Update(MainLoop_GetFrameCount());
  Gfx3d_SetViewport(NULL);
  func_0200c5d8(&data_022a7d5c);
  Camera_UpdateViewMatrix(&data_022a7d5c);
  Camera_BuildMatrices(&data_022a7d5c, &data_022a7cac);
  *REG_MTX_MODE = GEOM_TARGET_PROJ;
  *REG_MTX_IDENTITY = 0;
  func_0210b5b8(&data_022a7cac);
  Matrix_Expand43Into44(&data_022a7d5c.mtx43, &view);
  func_0210b5b8(&view);
  *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
  *REG_MTX_IDENTITY = 0;
}
