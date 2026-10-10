#include "types.h"
#include "game/BattleSpriteTask.h"

/* The sprite object the spawned task is placed at. */
typedef struct CueSource {
    u8 pad_00[0x38];
    s16 x;
    s16 y;
    s16 z;
} CueSource;

typedef struct CueSize {
    u8 pad_00[0x22];
    s16 width;
    s16 height;
} CueSize;

/* A task that waits for its sprite task to reach cue 5, then starts another
 * sprite task at the sprite's place. A guess from the fields it reads. */
typedef struct CueSpawnerTask {
    u8 pad_00[0x2c];
    CueSource *source;
    u8 pad_30[4];
    BattleSpriteTask *sprite;
    CueSize *size;
    u32 spawnCount;                    /* passed on as the second argument of func_ov002_02357e5c */
    u16 scale;                         /* 8.8 fixed point */
    u8 side;
} CueSpawnerTask;

#define CUE_SPAWN 5

extern u8 gMenuState[];
int Battle_GetMatchupValue(int side);
void func_ov002_0235643c(const u32 *table, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                         int a8, int a9, int a10, int a11, int a12, int a13);
void func_ov002_02357e5c(int side, int a1);
void Task_ResumeScript(void *task);

/* Needed to match: the original adds the offset into gMenuState at run time. */
#define MENU_WORD(offset, side) (*(u32 *)((u8 *)&gMenuState + (side) * 4 + (offset)))
#define SPRITE_TABLE(side) ((const u32 *)MENU_WORD(0x40, side))

void Battle_UpdateCueSpawner(CueSpawnerTask *task)
{
    int side;
    int scale;
    CueSize *size;
    CueSource *source;

    if (task->sprite->unk_5b == CUE_SPAWN) {
        task->sprite->unk_5b = 0;
        side = task->side;
        if (SPRITE_TABLE(side) != 0) {
            source = task->source;
            scale = task->scale;
            size = task->size;
            func_ov002_0235643c(SPRITE_TABLE(task->side), Battle_GetMatchupValue(side), source->x,
                                source->y, source->z, size->width * scale >> 8,
                                size->height * scale >> 8, -1, scale, scale,
                                *(u16 *)((u32)&gMenuState + side * 2 + 12), 8, 0, side);
        }
        if (MENU_WORD(0x48, task->side) != 0) {
            func_ov002_02357e5c(task->side, task->spawnCount);
        }
        task->spawnCount++;
    }
    if (task->sprite->unk_5c != 0) {
        Task_ResumeScript(task);
    }
}
