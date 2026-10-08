#ifndef GAME_TEXT_REF_H
#define GAME_TEXT_REF_H

#include "types.h"

/* A reference to one game string: an index inside a text group. The group
 * keys seen so far: 0xf000 CO text, 0xc000 general, 0x7c00 Force Rank names,
 * 0x7b00 Force Rank descriptions. */
typedef struct TextRef {
    u16 index;
    u16 group;
} TextRef;

#endif
