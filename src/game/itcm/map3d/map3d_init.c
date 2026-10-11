#include "types.h"
#include "sdk/fx.h"
#include "game/camera_pan.h"

extern u8 data_022a7cec[];
extern u8 data_022a7c6c[];
extern MtxFx44 data_022a7cac;
extern Camera data_022a7d5c;

void func_01ffd44c(void);
void func_01ffd32c(void *arg);
void Map3d_LoadTextures(void *arg);
void Gfx3d_LoadOrthoProjection(void *mtx);
void Gfx3d_LoadPerspectiveProjection(void *mtx);
void Camera_ResetToPreset0(Camera *camera);
void Camera_ResetToOrigin(void);
void func_020fbb0c(void);
void *func_020fbb44(void);

/* Sets up the 3D map: builds its tables and matrices, loads the orthographic
 * and perspective projections and puts the camera at preset 0. */
void Map3d_Init(void)
{
    func_01ffd44c();
    func_01ffd32c(data_022a7cec);
    Map3d_LoadTextures(data_022a7cec);
    Gfx3d_LoadOrthoProjection(data_022a7c6c);
    Gfx3d_LoadPerspectiveProjection(&data_022a7cac);
    Camera_ResetToPreset0(&data_022a7d5c);
    Camera_ResetToOrigin();
    func_020fbb0c();
    func_020fbb44();
}
