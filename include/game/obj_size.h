#ifndef GAME_OBJ_SIZE_H
#define GAME_OBJ_SIZE_H

#include "types.h"

/* Width and height in pixels of a 2D OBJ, indexed by its shape (OAM
 * attribute 0 bits 14-15) and size (attribute 1 bits 14-15). */
typedef struct ObjSize {
  s16 width;
  s16 height;
} ObjSize;

extern ObjSize gObjSizes[][4];

#endif
