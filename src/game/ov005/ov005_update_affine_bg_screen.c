#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/affine_params.h"
#include "game/ov005_task.h"
#include "game/task.h"
#include "game/unit_show_task.h"
#include "game/Unk_ov005_0236bb80.h"
#include "game/screen_contexts.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void* Mem_Alloc(u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Flush(void);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
u32* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(u32* sprite, int frame);
void SpriteAnim_Draw(u32* sprite, int x, int y);
void Palette_Darken(u8* palettes, int first, int count, int level);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
void Ov005Screen_WaitTaskThenResume(void);
void Ov005_ClearHasTaskB(void);
void Ov005_SetHasTaskB(void);
void Ov005_SetupDarkenedScreensWithSong(void);
void Ov005_SetupScreensWithAffineBg(void);
void Ov005_SetupScreensWithBlend(void);
void Ov005_UpdateScrollingTextPromptScreen(void);
void Ov005_UpdateSlideInTextScreen(void);
void Ov005_UpdateTextPromptScreen(void);
extern u8 data_02178b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_ov005_02350578(void);
void func_ov005_02352790(void);
void func_ov005_02353204(void);
void func_ov005_02353214(void);
extern u8 data_02179b08[];
extern u8 data_0217fb08[];

static TaskScriptCmd data_ov005_02368098[] = {
#include "data/ov005/data_ov005_02368098.inc.c"
};
static TaskScriptCmd data_ov005_023680c0[] = {
#include "data/ov005/data_ov005_023680c0.inc.c"
};
/* declared `u8 gOv005DarkenedSubLayers[]` in ov005_setup_darkened_screens_with_song.c: relocation at gOv005DarkenedSubLayers+0xc is not on a 4-byte field */
u32 gOv005DarkenedSubLayers[] = {
#include "data/ov005/gOv005DarkenedSubLayers.inc.c"
};
/* declared `u8 gOv005BlendSubLayers[]` in ov005_setup_screens_with_blend.c: relocation at gOv005BlendSubLayers+0xc is not on a 4-byte field */
u32 gOv005BlendSubLayers[] = {
#include "data/ov005/gOv005BlendSubLayers.inc.c"
};
/* declared `u8 data_ov005_023680f0[]` in ov005_setup_darkened_screens_with_song.c: relocation at data_ov005_023680f0+0xc is not on a 4-byte field */
u32 data_ov005_023680f0[] = {
#include "data/ov005/data_ov005_023680f0.inc.c"
};
/* declared `u8 gOv005AffineSubLayers[]` in ov005_setup_screens_with_affine_bg.c: relocation at gOv005AffineSubLayers+0xc is not on a 4-byte field */
u32 gOv005AffineSubLayers[] = {
#include "data/ov005/gOv005AffineSubLayers.inc.c"
};
TaskScriptCmd data_ov005_02368230[] = {
#include "data/ov005/data_ov005_02368230.inc.c"
};

/* Fades OBJ palette `palette` from black back to its saved colours in
 * block80, by the ease of the task's frame count. */
#define FADE_IN_OBJ_PALETTE(palette)                                         \
  {                                                                          \
    Unk_027c0280* saved0280 = data_027c0280;                                 \
    int level;                                                               \
    Screen_SetCurrentGfx(data_027c0288.unk_04);                              \
    level = Math_Ease(0, 0x10000, 0, task->frame, task->frames);             \
    Mem_Copy32((u8*)work->block80 + (palette) * 32,                          \
               data_027c0280->paletteBuffer + (palette) * 32, 32);           \
    Palette_Darken(data_027c0280->paletteBuffer, (palette), 1, level >> 12); \
    Palette_Flush();                                                         \
    Screen_SetCurrentGfx(saved0280);                                         \
  }

/* Counts the task's frame up to its frame count, then moves to `next`. */
#define ADVANCE_FRAME(next)          \
  if (task->frame < task->frames) {  \
    task->frame++;                   \
  }                                  \
  if (task->frame >= task->frames) { \
    task->step = (next);             \
  }

/* Update step of the screen Ov005_SetupScreensWithAffineBg sets up, all on
 * the sub screen. Step 0 makes four sprites from the file in unk_b4 at
 * (64,48), (192,48), (192,144), (64,144) and blacks out every palette. Steps
 * 10-41 fade in OBJ palettes 16 to 19, one sprite at a time, 80 frames each.
 * After a 64-frame wait, step 52 moves the sprites to (128,96) and starts
 * taskB. Step 60 hides all but BG2, starts taskA, and step 61 eases BG2's
 * scale from 512 to 256 (1.0) around (128,96). Every step then places the
 * sprites. */
void Ov005_UpdateAffineBgScreen(Ov005Task* task) {
  Unk_ov005_0236bb80* work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  AffineParams params;

  work = task->work;
  switch (task->step) {
    case 0:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      work->hasTaskA = 0;
      work->hasTaskB = 0;
      work->spriteHandles[0] = SpriteAnim_Create(work->unk_b4, 0, 2);
      work->spriteHandles[1] = SpriteAnim_Create(work->unk_b4, 0, 2);
      work->spriteHandles[2] = SpriteAnim_Create(work->unk_b4, 0, 2);
      work->spriteHandles[3] = SpriteAnim_Create(work->unk_b4, 0, 2);
      ((ShowSprite*)work->spriteHandles[0])->unk_24 = 0;
      ((ShowSprite*)work->spriteHandles[1])->unk_24 = 0;
      ((ShowSprite*)work->spriteHandles[2])->unk_24 = 0;
      ((ShowSprite*)work->spriteHandles[3])->unk_24 = 0;
      SpriteAnim_SetAnimation(work->spriteHandles[0], 0);
      SpriteAnim_SetAnimation(work->spriteHandles[1], 1);
      SpriteAnim_SetAnimation(work->spriteHandles[2], 2);
      SpriteAnim_SetAnimation(work->spriteHandles[3], 3);
      work->spritePos[0][0] = 64;
      work->spritePos[0][1] = 48;
      work->spritePos[1][0] = 192;
      work->spritePos[1][1] = 48;
      work->spritePos[2][0] = 192;
      work->spritePos[2][1] = 144;
      work->spritePos[3][0] = 64;
      work->spritePos[3][1] = 144;
      work->block80 = Mem_Alloc(0x400);
      Mem_Copy32(data_027c0280->paletteBuffer, work->block80, 0x400);
      Palette_Darken(data_027c0280->paletteBuffer, 0, 32, 16);
      Palette_Flush();
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->step = 10;
      break;
    case 10:
      task->frame = 0;
      task->frames = 80;
      task->step++;
      /* fall through */
    case 11:
      FADE_IN_OBJ_PALETTE(16);
      ADVANCE_FRAME(20);
      break;
    case 20:
      task->frame = 0;
      task->frames = 80;
      task->step++;
      /* fall through */
    case 21:
      FADE_IN_OBJ_PALETTE(17);
      ADVANCE_FRAME(30);
      break;
    case 30:
      task->frame = 0;
      task->frames = 80;
      task->step++;
      /* fall through */
    case 31:
      FADE_IN_OBJ_PALETTE(18);
      ADVANCE_FRAME(40);
      break;
    case 40:
      task->frame = 0;
      task->frames = 80;
      task->step++;
      /* fall through */
    case 41:
      FADE_IN_OBJ_PALETTE(19);
      ADVANCE_FRAME(50);
      break;
    case 50:
      task->frame = 0;
      task->frames = 64;
      task->step++;
      /* fall through */
    case 51:
      if (task->frame < task->frames) {
        task->frame++;
      }
      if (task->frame >= task->frames) {
        task->frame = 0;
        task->frames = 64;
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        work->taskB = Task_Create(data_ov005_023680c0, TASK_LIST(3));
        ((Ov005Task*)work->taskB)->work = work;
        work->hasTaskB = 1;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        task->step++;
      }
      break;
    case 52:
      work->spritePos[0][0] = Math_Ease(0, 64, 128, task->frame, task->frames);
      work->spritePos[0][1] = Math_Ease(0, 48, 96, task->frame, task->frames);
      work->spritePos[1][0] = Math_Ease(0, 192, 128, task->frame, task->frames);
      work->spritePos[1][1] = Math_Ease(0, 48, 96, task->frame, task->frames);
      work->spritePos[2][0] = Math_Ease(0, 192, 128, task->frame, task->frames);
      work->spritePos[2][1] = Math_Ease(0, 144, 96, task->frame, task->frames);
      work->spritePos[3][0] = Math_Ease(0, 64, 128, task->frame, task->frames);
      work->spritePos[3][1] = Math_Ease(0, 144, 96, task->frame, task->frames);
      ADVANCE_FRAME(60);
      break;
    case 60:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt |= 0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      work->taskA = Task_Create(data_ov005_02368098, TASK_LIST(3));
      ((Ov005Task*)work->taskA)->work = work;
      work->hasTaskA = 1;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->frame = 0;
      task->frames = 128;
      task->step++;
      /* fall through */
    case 61: {
      s16 scale;

      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      scale = (u16)Math_Ease(0, 512, 256, task->frame, task->frames);
      params.scaleX = scale;
      params.scaleY = scale;
      params.srcX = 128;
      params.centerX = 128;
      params.srcY = 96;
      params.centerY = 96;
      params.angle = 0;
      BgAffine_FromParams(&params, &data_027c0280->bg2Affine);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      ADVANCE_FRAME(100);
      break;
    }
    case 100:
      break;
  }

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  SpriteAnim_Draw(work->spriteHandles[0], work->spritePos[0][0],
                  work->spritePos[0][1]);
  SpriteAnim_Draw(work->spriteHandles[1], work->spritePos[1][0],
                  work->spritePos[1][1]);
  SpriteAnim_Draw(work->spriteHandles[2], work->spritePos[2][0],
                  work->spritePos[2][1]);
  SpriteAnim_Draw(work->spriteHandles[3], work->spritePos[3][0],
                  work->spritePos[3][1]);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
