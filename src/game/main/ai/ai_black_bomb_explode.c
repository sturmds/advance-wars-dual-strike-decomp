#include "types.h"

void AiBlackBomb_ChooseExplosionTile(void);

/* Queue routine set by AiAction_QueueBlackBombExplosions. */
void AiBlackBomb_Explode(void) { AiBlackBomb_ChooseExplosionTile(); }
