#include "types.h"

void Sound_PlaySe(int id);

/* Plays the jingle for a Score_ToRank result: one sound each for ranks
 * 5, 4 and 3, and a fourth for anything lower. */
void ResultRank_PlaySe(int rank) {
  switch (rank) {
    case 5:
      Sound_PlaySe(77);
      break;
    case 4:
      Sound_PlaySe(78);
      break;
    case 3:
      Sound_PlaySe(79);
      break;
    default:
      Sound_PlaySe(80);
      break;
  }
}
