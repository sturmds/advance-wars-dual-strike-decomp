#include "types.h"
#include "game/ai_unit_action.h"

/* Bubble sort of the first count entries of gAiQueue by their keys in
 * data_0229e234, smallest key first. */
void AiQueue_SortAscending(int count)
{
    int i;
    int j;
    u8 tmp;

    if (count < 2) {
        return;
    }
    for (i = 0; i <= count - 2; i++) {
        for (j = count - 2; j >= i; j--) {
            if (data_0229e234[j] > data_0229e234[j + 1]) {
                tmp = data_0229e234[j];
                data_0229e234[j] = data_0229e234[j + 1];
                data_0229e234[j + 1] = tmp;
                tmp = gAiQueue[j];
                gAiQueue[j] = gAiQueue[j + 1];
                gAiQueue[j + 1] = tmp;
            }
        }
    }
}
