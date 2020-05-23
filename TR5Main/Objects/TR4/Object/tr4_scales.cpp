#include "framework.h"
#include "tr4_scales.h"
#include "level.h"
#include "control.h"
#include "setup.h"
#include "items.h"
#include "lara.h"
#include "switch.h"
#include "sound.h"
#include "draw.h"
#include "tomb4fx.h"
#include "tr4_ahmet.h"

static short ScalesBounds[12] =
{
	0xFA80, 0xFA80, 0x0000, 0x0000, 0xFE00, 0x0200,
	0xF8E4, 0x071C, 0xEAAC, 0x1554, 0xF8E4, 0x071C
};

void ScalesControl(short itemNum)
{
	ITEM_INFO* item = &Items[itemNum];

	if (item->frameNumber != Anims[item->animNumber].frameEnd)
	{
		AnimateItem(item);
		return;
	}

	if (item->currentAnimState == 1 || item->itemFlags[1])
	{
		if (Objects[item->objectNumber].animIndex)
		{
			RemoveActiveItem(itemNum);
			item->status = ITEM_NOT_ACTIVE;
			item->itemFlags[1] = 0;

			AnimateItem(item);
			return;
		}

		if (RespawnAhmet((short)Lara.generalPtr))
		{
			short itemNos[8];
			int sw = GetSwitchTrigger(item, itemNos, 0);

			if (sw > 0)
			{
				while (Items[itemNos[sw]].objectNumber == ID_FLAME_EMITTER2)
				{
					if (--sw <= 0)
						break;
				}
				Items[itemNos[sw]].flags = 1024;
			}

			item->goalAnimState = 1;
		}

		AnimateItem(item);
	}

	int flags = 0;

	if (item->currentAnimState == 2)
	{
		flags = -512;
		RemoveActiveItem(itemNum);
		item->status = ITEM_NOT_ACTIVE;
	}
	else
	{
		flags = -1024;
		item->itemFlags[1] = 1;
	}

	short roomNumber = item->roomNumber;
	FLOOR_INFO* floor = GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber);
	GetFloorHeight(floor, item->pos.xPos, item->pos.yPos, item->pos.zPos);

	TestTriggers(TriggerIndex, TRUE, flags);
	AnimateItem(item);
}

void ScalesCollision(short itemNum, ITEM_INFO* l, COLL_INFO* coll)
{
	ITEM_INFO* item = &Items[itemNum];

	if (TestBoundsCollide(item, l, LARA_RAD))
	{
		if (l->animNumber != ANIMATION_LARA_WATERSKIN_EMPTY && l->animNumber != ANIMATION_LARA_WATERSKIN_POUR_ON_SCALE || item->currentAnimState != 1)
		{
			GlobalCollisionBounds.X1 = 640;
			GlobalCollisionBounds.X2 = 1280;
			GlobalCollisionBounds.Y1 = -1280;
			GlobalCollisionBounds.Y2 = 0;
			GlobalCollisionBounds.Z1 = -256;
			GlobalCollisionBounds.Z2 = 384;

			ItemPushLara(item, l, coll, 0, 2);

			GlobalCollisionBounds.X1 = -256;
			GlobalCollisionBounds.X2 = 256;

			ItemPushLara(item, l, coll, 0, 2);

			GlobalCollisionBounds.X1 = -1280;
			GlobalCollisionBounds.X2 = -640;

			ItemPushLara(item, l, coll, 0, 2);
		}
		else
		{
			short rotY = item->pos.yRot;
			item->pos.yRot = (short)(l->pos.yRot + ANGLE(45)) & 0xC000;

			ScalesBounds[0] = -1408;
			ScalesBounds[1] = -640;
			ScalesBounds[4] = -512;
			ScalesBounds[5] = 0;

			if (TestLaraPosition(ScalesBounds, item, l))
			{
				l->animNumber = ANIMATION_LARA_WATERSKIN_POUR_ON_SCALE;
				l->frameNumber = Anims[item->animNumber].frameBase;
				item->pos.yRot = rotY;
			}
			else if (l->frameNumber == Anims[ANIMATION_LARA_WATERSKIN_POUR_ON_SCALE].frameBase + 51)
			{
				SoundEffect(SFX_TR4_POUR, &l->pos, 0);
				item->pos.yRot = rotY;
			}
			else if (l->frameNumber == Anims[ANIMATION_LARA_WATERSKIN_POUR_ON_SCALE].frameBase + 74)
			{
				AddActiveItem(itemNum);
				item->status = ITEM_ACTIVE;

				if (l->itemFlags[3] < item->triggerFlags)
				{
					item->goalAnimState = 4;
					item->pos.yRot = rotY;
				}
				else if (l->itemFlags[3] == item->triggerFlags)
				{
					item->goalAnimState = 2;
					item->pos.yRot = rotY;
				}
				else
				{
					item->goalAnimState = 3;
				}
			}
			else
			{
				item->pos.yRot = rotY;
			}
		}
	}
	
	if (l->frameNumber >= Anims[ANIMATION_LARA_WATERSKIN_EMPTY].frameBase + 44 &&
		l->frameNumber <= Anims[ANIMATION_LARA_WATERSKIN_EMPTY].frameBase + 72 ||
		l->frameNumber >= Anims[ANIMATION_LARA_WATERSKIN_POUR_ON_SCALE].frameBase + 51 &&
		l->frameNumber <= Anims[ANIMATION_LARA_WATERSKIN_POUR_ON_SCALE].frameBase + 74)
	{
		PHD_VECTOR pos;
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;

		GetLaraJointPosition(&pos, LM_LHAND);

		DRIP_STRUCT* drip = &Drips[GetFreeDrip()];
		drip->x = pos.x;
		drip->y = pos.y;
		drip->z = pos.z;
		drip->on = 1;
		drip->r = (GetRandomControl() & 0xF) + 24;
		drip->g = (GetRandomControl() & 0xF) + 44;
		drip->b = (GetRandomControl() & 0xF) + 56;
		drip->yVel = (GetRandomControl() & 0x1F) + 32;
		drip->gravity = (GetRandomControl() & 0x1F) + 32;
		drip->life = (GetRandomControl() & 0x1F) + 16;
		drip->roomNumber = l->roomNumber;
	}
}