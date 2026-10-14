#ifndef MODULE_PARAMS_H
#define MODULE_PARAMS_H

#include "types.h"

/* The module parameters after the ARM9 crt0 (BuildInfo), which Entry and
 * do_autoload read. The linker script computes the autoload list's ends and
 * the end of main's .bss (ARM9_LAYOUT_SYMBOLS). */
typedef struct ModuleParams {
  /* crt0's autoload list: destination, size to copy and size to clear of
   * each block (ITCM, DTCM) */
  void* autoloadListStart;
  void* autoloadListEnd;
  void* autoloadStart; /* the blocks' data, one after another */
  void* bssStart;      /* main's .bss */
  void* bssEnd;
  void* compressedEnd; /* for MIi_UncompressBackward; NULL: not compressed */
  u32 sdkVersion;
  u32 nitroCode[2]; /* the "nitrocode" words that end the block */
} ModuleParams;

extern ModuleParams BuildInfo;

#endif
