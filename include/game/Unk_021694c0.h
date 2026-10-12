#ifndef GAME_UNK_021694C0_H
#define GAME_UNK_021694C0_H

#include "types.h"

/* Pointed to by data_021694c0; used by many functions near 0x020f1b0c-0x020f3604. */
typedef struct Unk_021694c0 {
  int sceneKind; /* picks the mode transition list (data_021695a4) */
  int previousMode;
  int mode;          /* current mode */
  int requestedMode; /* set by SceneMode_Request; SceneMode_Advance makes it current */
  int unk_10;
  int unk_14;
  int unk_18;
  int effectMode; /* EffectScene_Update switches on it (EffectScene_SetMode) */
  int effectFrames; /* frames in effectMode; EffectScene_SetMode clears it */
  int unk_24;
  int unk_28;
  int unk_2c;
  int unk_30;
  u8 pad_34[4];
  int unk_38;
  int unk_3c;
} Unk_021694c0;

/* Task of the effect scripts in the data_021694c0 scene. x and y start at
 * unk_24 and unk_28 of the data_021694c0 record or at a fixed point, and
 * place the task's sprite. */
typedef struct Unk_021694c0Task {
  u8 pad_00[0x2c];
  int x;
  int y;
  u8 pad_34[0x50 - 0x34];
  struct ShowSprite* sprite; /* handle from SpriteAnim_Create */
  int easeFrom;              /* Math_Ease arguments of the scroll tasks */
  int easeTo;
  int easeElapsed;
  int easeDuration;
  s16 frame;
} Unk_021694c0Task;

#endif
