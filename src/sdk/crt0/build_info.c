#include "types.h"
#include "text_data.h"

extern u32 data_02175180[];  /* autoload list: destination, size, .bss size */
extern u32 data_02175198[];  /* end of the autoload list */
extern u32 gOverlayLoaded[]; /* first word of main's .bss */

/* The values come from the ROM at build time (config/AWRE/data_inc.txt);
 * the words that hold addresses are written as the symbols above. */
#pragma section TEXT_DATA begin

/* Module parameters that Entry and do_autoload read: [0] and [1] bound the
 * autoload list, [2] is where the autoloaded code is stored, [3] and [4]
 * bound main's .bss, [5] is the end of the compressed part (0: not
 * compressed; Entry passes it to MIi_UncompressBackward), [6] is the library
 * version, and [7] and [8] are the "nitrocode" words that end the block. */
u32 BuildInfo[9] = {
#include "data/main/BuildInfo.inc.c"
};

#pragma section TEXT_DATA end
