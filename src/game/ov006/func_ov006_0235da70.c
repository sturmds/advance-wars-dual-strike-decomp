#include "types.h"

void func_ov006_02351828(int x, int y, int value);
int func_ov006_02351780(int x, int y, int value);
void Ov006_ResetNeighborsToSea(int x, int y);
void func_ov006_0235d7dc(int x, int y);
void func_ov006_0235dd24(int x, int y);
void func_ov006_0236146c(int x, int y);

/* Like func_ov006_0235da1c with more follow-up: value 42 (sea) stops after
 * the store; value 1 also calls Ov006_ResetNeighborsToSea. Then refreshes the cell
 * and redraws around it (func_ov006_0235d7dc, func_ov006_0236146c). */
void func_ov006_0235da70(int x, int y, int value)
{
    func_ov006_02351828(x, y, value);
    if (value == 42) {
        return;
    }
    if (value == 1 && func_ov006_02351780(x, y - 1, 1)) {
        func_ov006_02351828(x, y - 1, 1);
    }
    if (value == 1) {
        Ov006_ResetNeighborsToSea(x, y);
    }
    func_ov006_0235dd24(x, y);
    func_ov006_0235d7dc(x, y);
    func_ov006_0236146c(x, y);
}
