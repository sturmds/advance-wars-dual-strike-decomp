#include "types.h"
#include "text_data.h"
#include "game/overlay.h"

/* crt0's autoload list: destination, size and .bss size of ITCM and DTCM.
 * The linker script computes both ends (ARM9_LAYOUT_SYMBOLS). */
extern char AUTOLOAD_LIST_START[];
extern char AUTOLOAD_LIST_END[];
extern char MAIN_BSS_END[]; /* ARM9_LAYOUT_SYMBOLS */

/* The values come from the ROM at build time (config/AWRE/data_inc.txt);
 * the words that hold addresses are written as the symbols above.
 * gOverlayLoaded is the first word of main's .bss. */
#pragma section TEXT_DATA begin

/* Module parameters that Entry and do_autoload read: [0] and [1] bound the
 * autoload list, [2] is where the autoloaded code is stored, [3] and [4]
 * bound main's .bss, [5] is the end of the compressed part (0: not
 * compressed; Entry passes it to MIi_UncompressBackward), [6] is the library
 * version, and [7] and [8] are the "nitrocode" words that end the block. */
u32 BuildInfo[] = {
#include "data/main/BuildInfo.inc.c"
};

#pragma section TEXT_DATA end
