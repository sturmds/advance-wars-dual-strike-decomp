#include "sdk/os_arm7.h"

extern OSIrqFunction
    data_arm7_03807750[]; /* IRQ handler table, indexed by IE bit */

/*
 * Writes function into the IRQ table entry of every interrupt in mask. Unlike
 * OS_SetIrqFunction it writes the table even for the DMA and timer bits,
 * which normally go through their own slots.
 */
void Irq_WriteVectorEntries(OSIrqMask mask, OSIrqFunction function) {
  int bit;

  for (bit = 0; bit < 25; bit++) {
    if (mask & 1) {
      data_arm7_03807750[bit] = function;
    }
    mask >>= 1;
  }
}
