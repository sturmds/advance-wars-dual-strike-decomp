#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160ac0.h"

#include "game/Unk_02160abc.h"

/* Cast view of Ov003State.unk_1da: the phase this frame runs in and the
 * one asked for. func_ov003_02362334 asks for phase 7 (probably quit). */
typedef struct Ov003Phase {
    u8 current : 4;
    u8 next : 4;
} Ov003Phase;

#define PHASE(state) ((Ov003Phase *)&(state)->unk_1da)

#define PHASE_PLAY  0
#define PHASE_PAUSE 2

/* Pause menu buttons, 64 by 32 pixels at x 96: "resume" on top, "quit" below. */
#define PAUSE_BUTTON_X      96
#define PAUSE_RESUME_Y      64
#define PAUSE_QUIT_Y        96
#define PAUSE_BUTTON_WIDTH  64
#define PAUSE_BUTTON_HEIGHT 32

void Ov003_ReadInput(void);
void Ov003_AdvanceMode(void);
void func_ov003_02350e88(void);
void func_ov003_0235fee4(void);
void func_ov003_02364800(void);
void func_ov003_023509b8(void);
void Ov003_UpdateLinkStart(void);
void func_ov003_02350bdc(void);
int func_ov003_02362258(void);
int func_ov003_023621f4(void);
int Ov003_InputTouchedRect(Ov003Input *input, int x, int y, int width, int height);
void func_ov003_02362334(void);
void Screen_SetCurrentBoth(void *state);
void Oam_PutCells(int attr1, int attr0, const void *cells, int attr2, int tile);

extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern Ov003Input data_ov003_02365b04;
extern u8 data_027c0008;

/* Runs one frame of overlay 3, then the pause menu: START pauses and darkens
 * the screen; the top button (touch, or A/START on the cursor) resumes, the
 * bottom one calls func_ov003_02362334, probably quit. Up and down move the
 * cursor. */
void Ov003_Update(void)
{
    Ov003State *state;
    int current;

    Ov003_ReadInput();
    Ov003_AdvanceMode();
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    func_ov003_02350e88();
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    func_ov003_0235fee4();
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    func_ov003_02364800();
    func_ov003_023509b8();
    Ov003_UpdateLinkStart();
    func_ov003_02350bdc();
    if (func_ov003_02362258() == 1) {
        data_ov003_02364928->frameCount++;
    }
    if (func_ov003_023621f4() == 1) {
        data_ov003_02364928->unk_1cc++;
    }
    if (data_02160ac0->unk_7c != 0) {
        return;
    }

    state = data_ov003_02364928;
    current = PHASE(state)->current;
    if (current == PHASE(state)->next && current == PHASE_PLAY) {
        if (!(gKeypad->pressed & PAD_BUTTON_START)) {
            return;
        }
        PHASE(state)->next = PHASE_PAUSE;
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

    if (Ov003_InputTouchedRect(&data_ov003_02365b04, PAUSE_BUTTON_X, PAUSE_RESUME_Y,
                               PAUSE_BUTTON_WIDTH, PAUSE_BUTTON_HEIGHT) == 1) {
        PHASE(data_ov003_02364928)->next = PHASE_PLAY;
        data_027c0280->blendMode = 0;
        data_027c0280->blendEva = 16;
        data_027c0280->blendEvb = 0;
        data_027c0280->blendEvy = 0;
    } else if (Ov003_InputTouchedRect(&data_ov003_02365b04, PAUSE_BUTTON_X, PAUSE_QUIT_Y,
                                      PAUSE_BUTTON_WIDTH, PAUSE_BUTTON_HEIGHT) == 1) {
        data_02160abc->unk_0a = 0;
        func_ov003_02362334();
    } else {
        if (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN)) {
            data_ov003_02364928->pauseCursor++;
            data_ov003_02364928->pauseCursor %= 2;
        }
        if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_START)) {
            if (data_ov003_02364928->pauseCursor == 0) {
                PHASE(data_ov003_02364928)->next = PHASE_PLAY;
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
