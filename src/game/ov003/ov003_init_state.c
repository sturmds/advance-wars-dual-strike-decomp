#include "types.h"

typedef struct Unk_023519e0_0 Unk_023519e0_0;
typedef struct Unk_023519e0_1 Unk_023519e0_1;

struct Unk_023519e0_0 {
    u8 pad_00[0x7c];
    u8 unk_7c;
};

struct Unk_023519e0_1 {
    u8 pad_00[0x1df];
    u8 unk_1df;
};

extern Unk_023519e0_0 *data_02160ac0;
extern Unk_023519e0_1 *data_ov003_02364928;

void func_ov003_02352e88(void);
void func_ov003_02352090(void);
void func_ov003_02351da0(void);
void func_ov003_02351cf0(void);
void func_ov003_02353bd8(void);
void func_ov003_02351a5c(void);
void func_ov003_0235e22c(void);
u32 MainLoop_GetFrameCount(void);
void Ov003_RandSeed(u32 seed);
void Keypad_Init(void);

/* Seeds the random generator with the frame count, or with a fixed seed when
 * the byte at 0x7c of data_02160ac0 is set (same flag func_ov003_023622b8 reads). */
void Ov003_InitState(void)
{
    func_ov003_02352e88();
    func_ov003_02352090();
    func_ov003_02351da0();
    func_ov003_02351cf0();
    func_ov003_02353bd8();
    func_ov003_02351a5c();
    func_ov003_0235e22c();
    if (data_02160ac0->unk_7c == 0) {
        Ov003_RandSeed(MainLoop_GetFrameCount());
    } else {
        Ov003_RandSeed(0x31527396);
    }
    Keypad_Init();
    data_ov003_02364928->unk_1df &= ~2;
}
