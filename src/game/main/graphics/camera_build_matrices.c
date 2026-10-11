#include "types.h"
#include "sdk/fx.h"
#include "game/camera_pan.h"

void func_02109d88(MtxFx44 *a, MtxFx44 *b, MtxFx44 *dest);            /* SDK: 4x4 product */
void func_02109348(MtxFx33 *src, MtxFx33 *dest);                      /* SDK: 3x3 inverse */

/* Multiplies the camera's 4x3 matrix by mtx into mtx44, and stores the inverse
 * of its upper 3x3 part in mtx33. */
void Camera_BuildMatrices(Camera *camera, MtxFx44 *mtx)
{
    MtxFx44 expanded;
    MtxFx33 upper;

    Matrix_Expand43Into44(&camera->mtx43, &expanded);
    func_02109d88(&expanded, mtx, &camera->mtx44);
    Matrix_Take33From44(&camera->mtx44, &upper);
    func_02109348(&upper, &camera->mtx33);
}
