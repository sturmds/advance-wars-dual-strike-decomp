#include "types.h"
#include "sdk/gx.h"

/* Multiplies the current matrix by mtx's 3x3 rotation part (its first three
 * rows), leaving out the translation row. */
void Geom_MultMatrix43Rotation(MtxFx43* mtx) {
  Geom_MultMatrix33((MtxFx33*)mtx);
}
