#ifndef GAME_BGCONTROL_H
#define GAME_BGCONTROL_H

#include "types.h"

/* Layout of a DS BGxCNT register value (GBATEK). */
typedef struct BgControl {
  u16 priority : 2;
  u16 charBase : 4;
  u16 mosaic : 1;
  u16 colorMode : 1;
  u16 screenBase : 5;
  u16 extPltt : 1;
  u16 screenSize : 2;
} BgControl;

#endif
