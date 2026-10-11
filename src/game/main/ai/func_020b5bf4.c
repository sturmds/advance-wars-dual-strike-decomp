#include "types.h"

extern int (*data_0229e16c)(void);

int AiSeaTransport_ScoreFootUnitsVsProperties(void);
void AiTransport_HeadForUnloadTile(void);
void AiTransport_HeadForFlaggedUnloadTile(void);
void func_020b60e0(void);

/* Run by the sea transport routines when AI_STEP_BIT_08 is set, after the
 * tile search. The score function foot unit transports use gets an extra
 * step (AiTransport_HeadForFlaggedUnloadTile). Purpose of the steps not known yet. */
void func_020b5bf4(void)
{
    func_020b60e0();
    if (data_0229e16c == AiSeaTransport_ScoreFootUnitsVsProperties) {
        AiTransport_HeadForFlaggedUnloadTile();
    }
    AiTransport_HeadForUnloadTile();
}
