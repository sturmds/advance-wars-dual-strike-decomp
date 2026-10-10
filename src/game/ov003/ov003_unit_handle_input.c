#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"

int func_ov003_0235d518(int player, int kind, int terrain);
void func_ov003_02361fc0(int x1, int y1, int x2, int y2, int length, int *outX, int *outY);
void Ov003Unit_Move(Ov003Unit *unit, int dir, int distance, int straight);
int func_ov003_0235d940(int player);
void func_ov003_0235ca50(int player);
int Ov003_InputTouchedCircle(Ov003Input *input, int x, int y, int size);
void Ov003Unit_SpawnEffectTowardPoint(Ov003Unit *unit, int x, int y);
void Ov003Unit_SpawnEffectToward(Ov003Unit *unit, u8 dirX, u8 dirY);
void Ov003Unit_SpawnFacingEffect(Ov003Unit *unit);

extern u8 data_ov003_02365d10[];

#define MAP data_ov003_02364a88
#define TERRAIN_AT(unit) (data_ov003_02365d10[MAP->cells[((unit)->y >> 7) * MAP->width + ((unit)->x >> 7)]] & 31)

/* One frame of a player's unit: the d-pad moves it (diagonals through
 * func_ov003_02361fc0) at the speed for its kind and terrain and turns it, L or
 * a touch on the bottom-right corner calls func_ov003_0235ca50 for its player,
 * and A, Y or a touch fires. Kinds 1 to 3 fire while A or Y is held, the others
 * once per press. */
void Ov003Unit_HandleInput(Ov003Unit *unit, Keypad *pad, Ov003Input *input)
{
    BOOL touchUsed = FALSE;
    int straight = 0;                  /* 1 when moving along one axis only */
    int speed;
    int dx;
    int dy;

    if (unit->unk_17_6) {
        return;
    }
    if (unit->unk_17_7) {
        return;
    }
    speed = func_ov003_0235d518(unit->unk_12_3, unit->tile, TERRAIN_AT(unit));

    if (pad->unk_00 & PAD_KEY_LEFT) {
        if (pad->unk_00 & PAD_KEY_RIGHT) {
            pad->unk_00 &= ~(PAD_KEY_LEFT | PAD_KEY_RIGHT);
        }
    }
    if (pad->unk_00 & PAD_KEY_UP) {
        if (pad->unk_00 & PAD_KEY_DOWN) {
            pad->unk_00 &= ~(PAD_KEY_UP | PAD_KEY_DOWN);
        }
    }

    dx = 0;
    dy = 0;
    if ((pad->unk_00 & PAD_KEY_UP) && (pad->unk_00 & PAD_KEY_LEFT)) {
        func_ov003_02361fc0(0, 0, -1, -1, speed, &dx, &dy);
    } else if ((pad->unk_00 & PAD_KEY_UP) && (pad->unk_00 & PAD_KEY_RIGHT)) {
        func_ov003_02361fc0(0, 0, 1, -1, speed, &dx, &dy);
    } else if ((pad->unk_00 & PAD_KEY_DOWN) && (pad->unk_00 & PAD_KEY_LEFT)) {
        func_ov003_02361fc0(0, 0, -1, 1, speed, &dx, &dy);
    } else if ((pad->unk_00 & PAD_KEY_DOWN) && (pad->unk_00 & PAD_KEY_RIGHT)) {
        func_ov003_02361fc0(0, 0, 1, 1, speed, &dx, &dy);
    } else if (pad->unk_00 & PAD_KEY_UP) {
        dy = -speed;
        straight = 1;
    } else if (pad->unk_00 & PAD_KEY_DOWN) {
        dy = speed;
        straight = 1;
    } else if (pad->unk_00 & PAD_KEY_LEFT) {
        dx = -speed;
        straight = 1;
    } else if (pad->unk_00 & PAD_KEY_RIGHT) {
        dx = speed;
        straight = 1;
    }

    if (dx != 0 || dy != 0) {
        unit->unk_16++;
    }
    if (dx < 0) {
        Ov003Unit_Move(unit, OV003_DIR_LEFT, -dx, straight);
    }
    if (dx > 0) {
        Ov003Unit_Move(unit, OV003_DIR_RIGHT, dx, straight);
    }
    if (dy < 0) {
        Ov003Unit_Move(unit, OV003_DIR_UP, -dy, straight);
    }
    if (dy > 0) {
        Ov003Unit_Move(unit, OV003_DIR_DOWN, dy, straight);
    }

    switch (unit->unk_12_0) {
    case OV003_DIR_LEFT:
        if (dx < 0) {
            break;
        }
        if (dy < 0) {
            unit->unk_12_0 = OV003_DIR_UP;
        } else if (dy > 0) {
            unit->unk_12_0 = OV003_DIR_DOWN;
        } else if (dx > 0) {
            unit->unk_12_0 = OV003_DIR_RIGHT;
        }
        break;
    case OV003_DIR_RIGHT:
        if (dx > 0) {
            break;
        }
        if (dy > 0) {
            unit->unk_12_0 = OV003_DIR_DOWN;
        } else if (dy < 0) {
            unit->unk_12_0 = OV003_DIR_UP;
        } else if (dx < 0) {
            unit->unk_12_0 = OV003_DIR_LEFT;
        }
        break;
    case OV003_DIR_UP:
        if (dy < 0) {
            break;
        }
        if (dx < 0) {
            unit->unk_12_0 = OV003_DIR_LEFT;
        } else if (dx > 0) {
            unit->unk_12_0 = OV003_DIR_RIGHT;
        } else if (dy > 0) {
            unit->unk_12_0 = OV003_DIR_DOWN;
        }
        break;
    case OV003_DIR_DOWN:
        if (dy > 0) {
            break;
        }
        if (dx > 0) {
            unit->unk_12_0 = OV003_DIR_RIGHT;
        } else if (dx < 0) {
            unit->unk_12_0 = OV003_DIR_LEFT;
        } else if (dy < 0) {
            unit->unk_12_0 = OV003_DIR_UP;
        }
        break;
    }

    if (func_ov003_0235d940(unit->unk_12_3) == 1) {
        if (pad->pressed & PAD_BUTTON_L) {
            func_ov003_0235ca50(unit->unk_12_3);
        } else if (Ov003_InputTouchedCircle(input, 240, 176, 24) == 1) {
            func_ov003_0235ca50(unit->unk_12_3);
            touchUsed = TRUE;
        }
    }

    if (unit->unk_15 != 0) {
        return;
    }

    /* needed to match: + 255 rather than - 1 (kinds 1 to 3) */
    if ((u8)(unit->tile + 255) <= 2) {
        if (!touchUsed && (u8)(input->phase + 255) <= 1) { /* phase 1 or 2 */
            Ov003Unit_SpawnEffectTowardPoint(unit, input->x, input->y);
            return;
        }
        if (pad->unk_00 & PAD_BUTTON_A) {
            dx = 0;
            dy = 0;
            if (pad->unk_00 & PAD_KEY_LEFT) {
                dx = OV003_DIR_LEFT;
            }
            if (pad->unk_00 & PAD_KEY_RIGHT) {
                dx = OV003_DIR_RIGHT;
            }
            if (pad->unk_00 & PAD_KEY_UP) {
                dy = OV003_DIR_UP;
            }
            if (pad->unk_00 & PAD_KEY_DOWN) {
                dy = OV003_DIR_DOWN;
            }
            if (dx != 0 && dy != 0) {
                Ov003Unit_SpawnEffectToward(unit, dx, dy);
                return;
            }
            Ov003Unit_SpawnFacingEffect(unit);
            return;
        }
        if (pad->unk_00 & PAD_BUTTON_Y) {
            Ov003Unit_SpawnFacingEffect(unit);
        }
    } else {
        if (!touchUsed && input->phase == 1 && input->active == 1) {
            Ov003Unit_SpawnEffectTowardPoint(unit, input->x, input->y);
            return;
        }
        if (pad->pressed & PAD_BUTTON_A) {
            dx = 0;
            dy = 0;
            if (pad->unk_00 & PAD_KEY_LEFT) {
                dx = OV003_DIR_LEFT;
            }
            if (pad->unk_00 & PAD_KEY_RIGHT) {
                dx = OV003_DIR_RIGHT;
            }
            if (pad->unk_00 & PAD_KEY_UP) {
                dy = OV003_DIR_UP;
            }
            if (pad->unk_00 & PAD_KEY_DOWN) {
                dy = OV003_DIR_DOWN;
            }
            if (dx != 0 && dy != 0) {
                Ov003Unit_SpawnEffectToward(unit, dx, dy);
                return;
            }
            Ov003Unit_SpawnFacingEffect(unit);
            return;
        }
        if (pad->pressed & PAD_BUTTON_Y) {
            Ov003Unit_SpawnFacingEffect(unit);
        }
    }
}
