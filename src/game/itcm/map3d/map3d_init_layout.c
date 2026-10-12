#include "types.h"
#include "game/Unk_022a7c5c.h"

/* VRAM where the frames of the two map animations are kept; queued from
 * there to their texture offsets. */
#define ANIM_FRAMES_VRAM 0x06010000
#define ANIM2_FRAMES_VRAM 0x06610000

/* Fills in the texture image and palette offsets of a 3D map graphics
 * object. Fields the header types as pointers get casts where they hold
 * offsets. */
void Map3d_InitLayout(Unk_022a7c5c* gfx) {
  gfx->unk_00 = 0x3c60;
  gfx->mapTexOffset = 0x3c60;
  gfx->mapTex2Offset = 0x8b60;
  gfx->animDst = (u8*)0x9fe0;
  gfx->animFrames[0] = (u8*)ANIM_FRAMES_VRAM;
  gfx->animFrames[1] = (u8*)(ANIM_FRAMES_VRAM + 0x3180);
  gfx->animFrames[2] = (u8*)(ANIM_FRAMES_VRAM + 0x3180 * 2);
  gfx->animFrames[3] = (u8*)(ANIM_FRAMES_VRAM + 0x3180 * 3);
  gfx->texPaletteOffset = 32;
  gfx->anim2Frames[0] = (u8*)ANIM2_FRAMES_VRAM;
  gfx->anim2Frames[1] = (u8*)(ANIM2_FRAMES_VRAM + 0x4400);
  gfx->anim2Frames[2] = (u8*)(ANIM2_FRAMES_VRAM + 0x4400 * 2);
  gfx->anim2Dst = (u8*)0xd160;
  gfx->unk_34 = (u8*)0x220;
  gfx->unk_38 = 0x1680;
  gfx->unk_3c = 0x220;
  gfx->unk_40 = 0x21e0;
  gfx->unk_44 = (u8*)0x420;
  gfx->unk_48 = 0x31e0;
  gfx->unk_4c = 0x440;
  gfx->unk_50 = 0x3be0;
  gfx->unk_54 = 0x460;
  gfx->unk_58 = 0x480;
  gfx->unk_5c = 0x11560;
  gfx->unk_60 = 0x11d60;
  gfx->unk_64 = 0x4a0;
  gfx->unk_68 = (void*)0x12560;
  gfx->unk_6c = (void*)0x4e0;
}
