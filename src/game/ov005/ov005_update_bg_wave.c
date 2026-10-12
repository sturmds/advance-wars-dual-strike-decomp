#include "types.h"
#include "game/ov005_task.h"
#include "sdk/gx.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bb80.h"
#include "game/ov005_ripple.h"

void* Mem_Alloc(s32 size);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void DmaTasks_StartPair(s16* src1, s16* src2, volatile u16* dest1,
                        volatile u16* dest2, void (*onFrame1)(void),
                        void (*onFrame2)(void));
void Ov005Ripple_SwapYBuffer(void);
void Ov005Ripple_SwapXBuffer(void);

/* Per-frame update of a sub engine BG0 wave. The first step allocates
 * per-scanline offset tables (OV005_RIPPLE_LINES each) and starts an HBlank DMA pair
 * (DmaTasks_StartPair) that copies xLines and yLines into BG0HOFS and BG0VOFS,
 * with two buffers each; the callbacks swap them. Over 50 frames it then
 * alpha-blends the layer in on screen data_027c0288.unk_04. Every frame it
 * rebuilds the offsets as 8 pixels plus a sine wave whose phase moves on by
 * one, and writes them into the buffer the flags select. */
void Ov005_UpdateBgWave(Ov005Task* task) {
  Unk_ov005_0236bb80* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->frames = 50;
      work->xOffsets = Mem_Alloc(OV005_RIPPLE_LINES * sizeof(int));
      work->yOffsets = Mem_Alloc(OV005_RIPPLE_LINES * sizeof(int));
      work->xBufferA = Mem_Alloc(OV005_RIPPLE_LINES * sizeof(s16));
      work->yBufferA = Mem_Alloc(OV005_RIPPLE_LINES * sizeof(s16));
      work->xBufferB = Mem_Alloc(OV005_RIPPLE_LINES * sizeof(s16));
      work->yBufferB = Mem_Alloc(OV005_RIPPLE_LINES * sizeof(s16));
      work->xWaveAmplitude = 0x4000;
      work->xWaveStep = 0x1000;
      work->xWavePhase = 0;
      work->yWaveAmplitude = 0;
      work->yWaveStep = 0;
      work->yWavePhase = 0;
      work->xUsesBufferB = FALSE;
      work->yUsesBufferB = FALSE;
      work->xLines = work->xBufferA;
      work->yLines = work->yBufferA;
      Ov005Ripple_FillOffsets(work->xOffsets, 0x8000);
      Ov005Ripple_PackOffsets(work->xOffsets, work->xBufferA);
      Ov005Ripple_PackOffsets(work->xOffsets, work->xBufferB);
      Ov005Ripple_FillOffsets(work->yOffsets, 0x8000);
      Ov005Ripple_PackOffsets(work->yOffsets, work->yBufferA);
      Ov005Ripple_PackOffsets(work->yOffsets, work->yBufferB);
      DmaTasks_StartPair(work->xLines, work->yLines, REG_DB_BG0HOFS,
                         REG_DB_BG0VOFS, Ov005Ripple_SwapXBuffer,
                         Ov005Ripple_SwapYBuffer);
      task->step++;
    case 1:
      work->blendEva = Math_Ease(0, 0, 0x8000, task->frame, task->frames);
      work->blendEvb = Math_Ease(0, 0x10000, 0x8000, task->frame, task->frames);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = work->blendEva >> 12;
      data_027c0280->blendEvb = work->blendEvb >> 12;
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame >= task->frames) {
        task->step++;
        break;
      }
      if (task->frame < task->frames) {
        task->frame++;
      }
      break;
    case 2:
      work->unk_28 = TRUE;
      break;
  }

  work->xWavePhase += 0x1000;
  work->yWavePhase += 0x1000;
  Ov005Ripple_FillOffsets(work->xOffsets, 0x8000);
  Ov005Ripple_AddWave(work->xOffsets, work->xWaveAmplitude, work->xWaveStep,
                      work->xWavePhase);
  Ov005Ripple_FillOffsets(work->yOffsets, 0x8000);
  Ov005Ripple_AddWave(work->yOffsets, work->yWaveAmplitude, work->yWaveStep,
                      work->yWavePhase);
  if (!work->xUsesBufferB) {
    Ov005Ripple_PackOffsets(work->xOffsets, work->xBufferA);
  } else {
    Ov005Ripple_PackOffsets(work->xOffsets, work->xBufferB);
  }
  if (!work->yUsesBufferB) {
    Ov005Ripple_PackOffsets(work->yOffsets, work->yBufferA);
  } else {
    Ov005Ripple_PackOffsets(work->yOffsets, work->yBufferB);
  }
}
