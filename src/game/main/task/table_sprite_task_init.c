#include "types.h"
#include "game/task.h"
#include "game/table_sprite_task.h"

/* Fills a Sprite_CreateTask task from the table: position, scale, tile and
 * the word at index + 1. */
void TableSpriteTask_Init(Task* task, const u32* table, int index, int x, s16 y,
                          s16 e, s16 scaleX, s16 scaleY, u16 tile, u8 g) {
  TableSpriteTask* sprite = (TableSpriteTask*)task;

  sprite->unk_2c = 0;
  sprite->unk_30 = table[0];
  table += index;
  sprite->unk_38 = table[1];
  sprite->unk_3c = sprite->unk_38;
  sprite->unk_34 = 0;
  sprite->unk_40 = 0;
  sprite->unk_44 = 0;
  sprite->unk_48 = 0;
  sprite->unk_4c = x;
  sprite->unk_4e = y;
  sprite->unk_50 = e;
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  sprite->tile = tile;
  sprite->unk_5a = g;
  sprite->unk_5b = 0xff;
  sprite->flags &= ~1;
  sprite->flags |= 2;
  sprite->flags &= ~4;
  sprite->flags &= ~8;
  sprite->unk_52 = 0;
}
