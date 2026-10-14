#ifndef GAME_TEXT_REF_H
#define GAME_TEXT_REF_H

#include "types.h"

/* A reference to one game string: an index inside a text group. The group
 * keys seen so far: 0xf000 CO text, 0xc000 general, 0x7c00 Force Rank names,
 * 0x7b00 Force Rank descriptions. The text lookup (Text_Get)
 * takes both halves as one u32, id. */
typedef union TextRef {
  struct {
    u16 index;
    u16 group;
  };
  u32 id;
} TextRef;

/* One text bank: ids from firstId upward index into strings (the low 24 bits). */
typedef struct TextBank {
  u32 firstId;
  const void** strings; /* 0 ends the list */
} TextBank;

typedef char TextBank_size_check[sizeof(TextBank) == 8 ? 1 : -1];

#endif
