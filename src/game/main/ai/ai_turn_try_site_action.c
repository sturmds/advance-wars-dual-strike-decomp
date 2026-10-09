#include "types.h"
#include "game/ai_site.h"

int func_0209658c(void);
void func_02094d94(int site);
int func_02093ef4(int site);
void func_0209316c(int site);

/* The kind byte of each AiSite record (data_0229e5bc + 3); the original
 * addresses it through its own symbol. */
extern u8 data_0229e5bf[][sizeof(AiSite)];

/* End-actions step 2. func_0209658c picks a site record (-1: none), then
 * func_02094d94 fills the range map from it. If func_02093ef4 finds nothing
 * for the site, its kind becomes 0xfe, else func_0209316c queues an action
 * for it. Returns 1 when it picked a site. */
BOOL AiTurn_TrySiteAction(void)
{
    int site = func_0209658c();

    if (site == -1) {
        return 0;
    }
    func_02094d94(site);
    if (func_02093ef4(site) != -1) {
        func_0209316c(site);
        return 1;
    }
    data_0229e5bf[site][0] = 0xfe;
    return 1;
}
