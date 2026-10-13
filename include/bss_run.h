#ifndef BSS_RUN_H
#define BSS_RUN_H

/* mwccarm sorts a file's .bss variables by size, so one section can only
 * hold variables whose sizes go up in address order. A file whose .bss in
 * the ROM goes down in size somewhere puts each later run between
 *
 *   #pragma section BSS_RUN1 begin
 *   #pragma section BSS_RUN1 end
 *
 * The linker script places .bss.run1 after the file's .bss, then
 * .bss.run2 and so on (tools/bss.py lcf). objdiff reads .bss.runN as .bss.
 * tools/bss.py writes these files. */
/* One line each: clang-format would break them in three. */
/* clang-format off */
#pragma define_section BSS_RUN1 ".data" ".bss.run1" abs32 RW
#pragma define_section BSS_RUN2 ".data" ".bss.run2" abs32 RW
#pragma define_section BSS_RUN3 ".data" ".bss.run3" abs32 RW
#pragma define_section BSS_RUN4 ".data" ".bss.run4" abs32 RW
#pragma define_section BSS_RUN5 ".data" ".bss.run5" abs32 RW
#pragma define_section BSS_RUN6 ".data" ".bss.run6" abs32 RW
#pragma define_section BSS_RUN7 ".data" ".bss.run7" abs32 RW
#pragma define_section BSS_RUN8 ".data" ".bss.run8" abs32 RW
#pragma define_section BSS_RUN9 ".data" ".bss.run9" abs32 RW
#pragma define_section BSS_RUN10 ".data" ".bss.run10" abs32 RW
#pragma define_section BSS_RUN11 ".data" ".bss.run11" abs32 RW
#pragma define_section BSS_RUN12 ".data" ".bss.run12" abs32 RW
#pragma define_section BSS_RUN13 ".data" ".bss.run13" abs32 RW
#pragma define_section BSS_RUN14 ".data" ".bss.run14" abs32 RW
#pragma define_section BSS_RUN15 ".data" ".bss.run15" abs32 RW
/* clang-format on */

#endif
