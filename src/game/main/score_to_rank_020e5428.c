#include "types.h"

int Score_ToRank_020e5428(int value) {
  if (value < 200) {
    return 2;
  }
  if (value < 250) {
    return 3;
  }
  if (value < 280) {
    return 4;
  }
  return 5;
}
