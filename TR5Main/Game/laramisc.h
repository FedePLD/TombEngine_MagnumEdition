#pragma once
#include "..\Global\global.h"

void GetLaraDeadlyBounds();
void DelAlignLaraToRope(ITEM_INFO* item);
void InitialiseLaraAnims(ITEM_INFO* item);
void InitialiseLaraLoad(short itemNumber);
void InitialiseLara(int restore);
void LaraControl(short itemNumber);
void LaraCheat(ITEM_INFO* item, COLL_INFO* coll);
void LaraInitialiseMeshes();
void AnimateLara(ITEM_INFO* item);