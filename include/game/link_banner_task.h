#ifndef GAME_LINK_BANNER_TASK_H
#define GAME_LINK_BANNER_TASK_H

#include "types.h"

/* Task of a text banner on the link play player list screen. The first
 * 0x2c bytes belong to the task system. LinkBanner_Create fills the
 * fields; the banner's scripted steps then reuse the second and third. */
typedef struct LinkBannerTask {
  u8 pad_00[0x2c];
  int textId; /* text shown in the banner */
  int timer;  /* LinkBanner_ShrinkBox counts it down to 0 */
  int width;  /* banner width in tiles, even, set by LinkBanner_Init */
  int unk_38;
} LinkBannerTask;

#endif
