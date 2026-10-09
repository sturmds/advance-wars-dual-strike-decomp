#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

/* Resets each AiSite.kind of 254 or more to 127. Purpose unknown. */
void func_0209378c(void)
{
    int i;
    AiSite *site = data_0229e5bc;

    for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->kind >= 254) {
            site->kind = 127;
        }
    }
}
