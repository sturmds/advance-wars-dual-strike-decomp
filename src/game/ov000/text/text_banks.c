#include "types.h"
#include "game/text_ref.h"

/* The banks Text_Get looks ids up in, from banks.yaml: tools/text.py pack
 * writes the tables' declarations and the values (docs/text.md). */
#include "text/text_banks.h"

TextBank gTextBanks[] = {
#include "text/text_banks.inc"
};
