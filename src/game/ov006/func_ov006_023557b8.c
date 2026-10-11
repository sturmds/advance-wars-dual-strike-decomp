#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

typedef struct ButtonPos {
    u8 x;
    u8 y;
} ButtonPos;

extern ButtonPos data_ov006_0236b6e8[5];
extern ButtonPos data_ov006_0236b6f4[5];
extern u8 data_ov006_0236b6c0[];
extern u8 data_ov006_0236b6c8[];
extern u8 data_ov006_0236b6d0[];
extern u8 data_ov006_0236b6d8[];
extern u8 data_ov006_0236b6e0[];

#define OV6 data_ov006_0236d640

static inline void QueueSprite(int x, int y, void *data, BOOL affine)
{
    Sprite_Queue(0, x & 0x1ff, (y & 0xff) | 0x400 | (affine ? 0x100 : 0), data, 0x1000, 0);
}

/* Queues the icons of the tool buttons that Ov006_AddToolButtons adds, 8
 * pixels inside each button; the fifth only with flag 1 of unk_02, which
 * also picks the position table. Not matching yet: the four saved
 * registers come out in the opposite order (affine and pos should get r4
 * and r5). */
void func_ov006_023557b8(void)
{
    u16 flags = OV6->unk_02;
    BOOL affine = (flags & 8) ? 1 : 0;
    ButtonPos *pos;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    if (flags & 1) {
        pos = data_ov006_0236b6e8;
    } else {
        pos = data_ov006_0236b6f4;
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    QueueSprite((pos[0].x + 8) & 0x1ff, (pos[0].y + 8) & 0xff, data_ov006_0236b6e0, affine);
    QueueSprite((pos[1].x + 8) & 0x1ff, (pos[1].y + 8) & 0xff, data_ov006_0236b6c8, affine);
    QueueSprite((pos[2].x + 8) & 0x1ff, (pos[2].y + 8) & 0xff, data_ov006_0236b6d8, affine);
    QueueSprite((pos[3].x + 8) & 0x1ff, (pos[3].y + 8) & 0xff, data_ov006_0236b6d0, affine);
    if (OV6->unk_02 & 1) {
        QueueSprite((pos[4].x + 8) & 0x1ff, (pos[4].y + 8) & 0xff, data_ov006_0236b6c0, affine);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
