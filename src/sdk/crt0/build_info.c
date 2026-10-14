#include "types.h"
#include "text_data.h"
#include "game/overlay.h"
#include "sdk/module_params.h"

/* crt0's autoload list: destination, size and .bss size of ITCM and DTCM.
 * The linker script computes both ends (ARM9_LAYOUT_SYMBOLS). */
extern char AUTOLOAD_LIST_START[];
extern char AUTOLOAD_LIST_END[];
extern char MAIN_BSS_END[]; /* ARM9_LAYOUT_SYMBOLS */

/* The values come from the ROM at build time (config/AWRE/data_inc.txt);
 * the words that hold addresses are written as the symbols above.
 * gOverlayLoaded is the first word of main's .bss. */
#pragma section TEXT_DATA begin

ModuleParams BuildInfo = {
#include "data/main/BuildInfo.inc.c"
};

#pragma section TEXT_DATA end

typedef char ModuleParams_size_check[sizeof(ModuleParams) == 0x24 ? 1 : -1];
