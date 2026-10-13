#include "types.h"
#include "sdk/fx.h"
#include "sdk/mi.h"
#include "game/camera_pan.h"

void MTX_Concat43(MtxFx43* a, MtxFx43* b, MtxFx43* dest); /* SDK: 4x3 product */
void Matrix_Invert3x3(MtxFx33* src, MtxFx33* dest);       /* SDK: 3x3 inverse */

extern s16 data_0214d4c4[]; /* a sine and cosine pair per 16 angle units */

#define SIN(angle) data_0214d4c4[((angle) >> 4) * 2]
#define COS(angle) data_0214d4c4[((angle) >> 4) * 2 + 1]

/* Builds the camera's view matrix mtx43 from its rotation and position, and
 * stores the inverse of its rotation part in invRot. */
void Camera_UpdateViewMatrix(Camera* camera) {
  MtxFx43 rotX;
  MtxFx43 rotY;
  MtxFx43 rotZ;
  MtxFx33 rot;

  MTX_RotX43_(&rotX, SIN(camera->rotX), COS(camera->rotX));
  MTX_RotY43_(&rotY, SIN(camera->rotY), COS(camera->rotY));
  Matrix_SetZTurn43FromSinCos(&rotZ, SIN(camera->rotZ), COS(camera->rotZ));
  MTX_Identity43(&camera->mtx43);
  MTX_Concat43(&camera->mtx43, &rotX, &camera->mtx43);
  MTX_Concat43(&camera->mtx43, &rotY, &camera->mtx43);
  MTX_Concat43(&camera->mtx43, &rotZ, &camera->mtx43);
  Matrix_MoveAlongLocalAxes43(&camera->mtx43, &camera->mtx43, -camera->x,
                              -camera->y, -camera->z);
  MI_Copy36B(&camera->mtx43, &rot);
  Matrix_Invert3x3(&rot, &camera->invRot);
}
