#include "types.h"
#include "game/tab_list.h"

extern TabListView gMenuState;
void Sound_PlaySe(int id);
void TabList_RebuildButtons(void);
int TabList_GetPageCount(int tab);
void func_02071494(void);

/* Shows the next page of the current tab, wrapping to the first page. */
void TabList_NextPage(void)
{
    int pageCount = TabList_GetPageCount(gMenuState.tab);

    gMenuState.pageOfTab[gMenuState.tab]++;
    if (gMenuState.pageOfTab[gMenuState.tab] >= pageCount) {
        gMenuState.pageOfTab[gMenuState.tab] = 0;
    }
    Sound_PlaySe(0x10002);
    TabList_RebuildButtons();
    func_02071494();
}
