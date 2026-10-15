#include "types.h"
#include "game/oam_cells.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160ac0.h"

#include "game/Unk_02160abc.h"

/* func_ov003_02362334 asks for phase 7 (probably quit). */

#define PHASE_PLAY 0
#define PHASE_PAUSE 2

/* Pause menu buttons, 64 by 32 pixels at x 96: "resume" on top, "quit" below. */
#define PAUSE_BUTTON_X 96
#define PAUSE_RESUME_Y 64
#define PAUSE_QUIT_Y 96
#define PAUSE_BUTTON_WIDTH 64
#define PAUSE_BUTTON_HEIGHT 32

void Ov003_ReadInput(void);
void Ov003_AdvancePhase(void);
void func_ov003_02350e88(void);
void Ov003_DrawHudSprites(void);
void Ov003_PlayPendingSound(void);
void Ov003_DrawTimeLeft(void);
void Ov003_UpdateLinkStart(void);
void Ov003_ResetSlotStates(void);
int Ov003_IsPhase0OrAbove6(void);
int Ov003_IsPhase0OrAbove7(void);
int Ov003_InputTouchedRect(Ov003Input* input, int x, int y, int width,
                           int height);
void func_ov003_02362334(void);
void Screen_SetCurrentBoth(void* state);
void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

extern Ov003Input data_ov003_02365b04;

/* Runs one frame of overlay 3, then the pause menu: START pauses and darkens
 * the screen; the top button (touch, or A/START on the cursor) resumes, the
 * bottom one calls func_ov003_02362334, probably quit. Up and down move the
 * cursor. */
void Ov003_Update(void) {
  Ov003State* state;
  int current;

  Ov003_ReadInput();
  Ov003_AdvancePhase();
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  func_ov003_02350e88();
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Ov003_DrawHudSprites();
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Ov003_PlayPendingSound();
  Ov003_DrawTimeLeft();
  Ov003_UpdateLinkStart();
  Ov003_ResetSlotStates();
  if (Ov003_IsPhase0OrAbove6() == 1) {
    data_ov003_02364928->frameCount++;
  }
  if (Ov003_IsPhase0OrAbove7() == 1) {
    data_ov003_02364928->unk_1cc++;
  }
  if (data_02160ac0->unk_7c != 0) {
    return;
  }

  state = data_ov003_02364928;
  current = state->phase;
  if (current == state->nextPhase && current == PHASE_PLAY) {
    if (!(gKeypad->pressed & PAD_BUTTON_START)) {
      return;
    }
    state->nextPhase = PHASE_PAUSE;
    /* darken everything but sprites */
    data_027c0280->blendMode = 3;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 8;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 0;
    data_ov003_02364928->pauseCursor = 0;
    return;
  }
  if (current != PHASE_PAUSE) {
    return;
  }

  if (Ov003_InputTouchedRect(&data_ov003_02365b04, PAUSE_BUTTON_X,
                             PAUSE_RESUME_Y, PAUSE_BUTTON_WIDTH,
                             PAUSE_BUTTON_HEIGHT) == 1) {
    data_ov003_02364928->nextPhase = PHASE_PLAY;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
  } else if (Ov003_InputTouchedRect(&data_ov003_02365b04, PAUSE_BUTTON_X,
                                    PAUSE_QUIT_Y, PAUSE_BUTTON_WIDTH,
                                    PAUSE_BUTTON_HEIGHT) == 1) {
    data_02160abc->unk_0a = 0;
    func_ov003_02362334();
  } else {
    if (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN)) {
      data_ov003_02364928->pauseCursor++;
      data_ov003_02364928->pauseCursor %= 2;
    }
    if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_START)) {
      if (data_ov003_02364928->pauseCursor == 0) {
        data_ov003_02364928->nextPhase = PHASE_PLAY;
        data_027c0280->blendMode = 0;
        data_027c0280->blendEva = 16;
        data_027c0280->blendEvb = 0;
        data_027c0280->blendEvy = 0;
      } else {
        data_02160abc->unk_0a = 0;
        func_ov003_02362334();
      }
    }
  }

  /* the button under the cursor uses palette 0, the other palette 1 */
  if (data_ov003_02364928->pauseCursor == 0) {
    Oam_PutCells(PAUSE_BUTTON_X, PAUSE_RESUME_Y, &data_027c0008, 0, 0x538);
    Oam_PutCells(PAUSE_BUTTON_X, PAUSE_QUIT_Y, &data_027c0008, 0x1000, 0x558);
  } else {
    Oam_PutCells(PAUSE_BUTTON_X, PAUSE_RESUME_Y, &data_027c0008, 0x1000, 0x538);
    Oam_PutCells(PAUSE_BUTTON_X, PAUSE_QUIT_Y, &data_027c0008, 0, 0x558);
  }
}
