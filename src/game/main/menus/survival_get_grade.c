#include "types.h"

/* Grades a saved Survival result for the mode variant: 5 is best, 2 lowest.
 * Modes 0, 1 and 2 compare the value (time, funds or days; guess: what was
 * left of the budget) with three thresholds; the other modes compare it
 * with 10, 15 and 20 (guess: maps cleared). */
int Survival_GetGrade(int mode, u32 value) {
  switch (mode) {
    case 0:
      if (value >= 32400) {
        return 5;
      }
      if (value >= 21600) {
        return 4;
      }
      if (value >= 10800) {
        return 3;
      }
      return 2;
    case 2:
      if (value >= 25) {
        return 5;
      }
      if (value >= 15) {
        return 4;
      }
      if (value >= 5) {
        return 3;
      }
      return 2;
    case 1:
      if (value >= 50000) {
        return 5;
      }
      if (value >= 25000) {
        return 4;
      }
      if (value >= 10000) {
        return 3;
      }
      return 2;
    default:
      if (value >= 20) {
        return 5;
      }
      if (value >= 15) {
        return 4;
      }
      if (value >= 10) {
        return 3;
      }
      return 2;
  }
}
