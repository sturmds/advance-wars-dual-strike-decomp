#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern int data_ov000_023504e0[];
extern u8 data_ov000_022dabfc[];
extern u8 data_ov000_022db3fc[];

void Screen_SetCurrentGfx(void *state);
void Mem_CopyAuto(const void *src, void *dst, u32 size);
void Palette_Load(const void *src, u32 offset, u32 size);
void func_ov000_022afbf0(void);
void func_ov000_022b14bc(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_ov000_022afee8(void)
{
    int *entry;
    Unk_02080440 *screen;
    int screenIndex;
    int i;
    Unk_027c0280 *savedGfx;
    entry = data_ov000_023504e0;
    screen = data_027c027c;
    screenIndex = data_027c0284;

    for (i = 1; i < 5; i++, entry += 3) {
        if (screen->unk_48[screenIndex]->players[i].controller != PLAYER_CONTROLLER_NONE) {
            entry[0] = -1;
            entry[1] = -1;
            entry[2] = -1;
        }
    }
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_CopyAuto(data_ov000_022dabfc, data_027c0280->unk_e4 + 0x4000, 0x800);
    Palette_Load(data_ov000_022db3fc, 32, 352);
    func_ov000_022afbf0();
    func_ov000_022b14bc();
    Screen_SetCurrentGfx(savedGfx);
}
