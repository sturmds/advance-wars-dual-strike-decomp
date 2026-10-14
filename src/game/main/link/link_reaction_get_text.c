#include "types.h"

extern u8 data_0229db54[][6][33];

/* The text a player (link slot, from 1) set for a reaction button, 33 bytes. */
u8* LinkReaction_GetText(int slot, int button) {
  return data_0229db54[slot - 1][button];
}
