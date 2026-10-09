#include "types.h"
#include "game/Unk_02080440.h"

void Sound_PlaySe(int id);

/* Task started here; the first 0x29 bytes belong to the task system. */
typedef struct Unk_020d5b58Task {
    u8 pad_00[0x29];
    u8 unk_29;
    u8 unk_2a;
    u8 pad_2b;
    u16 unk_2c;
    u16 unk_2e;
    u16 unk_30;
} Unk_020d5b58Task;

/* One CO's row of data_021536ec, 0x220 bytes: for each kind (0-3), 64 u16
 * with the sound effect ids in its first two entries, picked by the
 * counter. The last 0x20 bytes are not read here. */
typedef struct CoSoundRow {
    u16 byKind[4][64];
    u16 unk_200[16];
} CoSoundRow;

Unk_020d5b58Task *Task_Create(const void *script, void *parentOrList);

extern const u32 data_02166ed4[];
extern const u32 data_02166efc[];
extern CoSoundRow data_021536ec[];
extern int data_022a6c78;
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Purpose unknown (no direct callers). Starts a task from data_02166efc when
 * kind is 2, else from data_02166ed4, and plays a sound effect of the current
 * player's front CO picked by kind and a counter that alternates 0 and 1. */
void func_020d5b58(u8 a, u8 b, u16 c, int kind)
{
    Unk_020d5b58Task *task;

    data_022a6c78++;
    if (data_022a6c78 > 1) {
        data_022a6c78 = 0;
    }
    if (kind == 2) {
        task = Task_Create(data_02166efc, (void *)3);
    } else {
        task = Task_Create(data_02166ed4, (void *)3);
    }
    Sound_PlaySe(data_021536ec[GAME->players[GAME->currentPlayer].front.id & PLAYER_CO_ID_MASK].byKind[kind][data_022a6c78]);
    task->unk_29 = a;
    task->unk_2a = b;
    task->unk_2c = c;
    task->unk_2e = 0;
    task->unk_30 = 0;
}
