#include "framework.h"
#include "Objects/TR2/Entity/tr2_sword_guardian.h"

#include "Game/animation.h"
#include "Game/control/box.h"
#include "Game/control/lot.h"
#include "Game/effects/effects.h"
#include "Game/effects/tomb4fx.h"
#include "Game/items.h"
#include "Game/itemdata/creature_info.h"
#include "Game/Lara/lara.h"
#include "Sound/sound.h"
#include "Specific/level.h"

BITE_INFO swordBite = { 0, 37, 550, 15 };

void InitialiseSwordGuardian(short itemNum)
{
	ANIM_STRUCT* anim;
	ITEM_INFO* item;

	item = &g_Level.Items[itemNum];

	ClearItem(itemNum);
}

static void SwordGuardianFly(ITEM_INFO* item)
{
	PHD_VECTOR pos;

	pos.x = (GetRandomControl() * 256 / 32768) + item->Position.xPos - 128;
	pos.y = (GetRandomControl() * 256 / 32768) + item->Position.yPos - 256;
	pos.z = (GetRandomControl() * 256 / 32768) + item->Position.zPos - 128;

	TriggerGunSmoke(pos.x, pos.y, pos.z, 1, 1, 1, 1, LaraWeaponType::GrenadeLauncher, 32);
	SoundEffect(SFX_TR2_WARRIOR_HOVER, &item->Position, 0);
}

void SwordGuardianControl(short itemNum)
{
	if (!CreatureActive(itemNum))
		return;

	ITEM_INFO* item;
	CreatureInfo* sword;
	AI_INFO info;
	short angle, head, torso;
	bool lara_alive;

	item = &g_Level.Items[itemNum];
	sword = (CreatureInfo*)item->Data;
	angle = head = torso = 0;
	lara_alive = (LaraItem->HitPoints > 0);

	if (item->HitPoints <= 0)
	{
		if (item->ActiveState != 12)
		{
			//item->meshBits >>= 1;
			SoundEffect(SFX_TR4_EXPLOSION1, &LaraItem->Position, 0);
			SoundEffect(SFX_TR4_EXPLOSION2, &LaraItem->Position, 0);
			//item->meshBits = 0xFFFFFFFF;
			//item->objectNumber = ID_SAS;
			ExplodingDeath(itemNum, -1, 256);
			//item->objectNumber = ID_SWAT;
			DisableEntityAI(itemNum);
			KillItem(itemNum);
			//item->status = ITEM_DESACTIVATED;
			//item->flags |= ONESHOT;
			item->ActiveState = 12;
		}
		return;
	}
	else
	{
		sword->LOT.Step = STEP_SIZE;
		sword->LOT.Drop = -STEP_SIZE;
		sword->LOT.Fly = NO_FLYING;
		sword->LOT.Zone = ZONE_BASIC;
		CreatureAIInfo(item, &info);

		if (item->ActiveState == 8)
		{
			if (info.zoneNumber != info.enemyZone)
			{
				sword->LOT.Step = WALL_SIZE * 20;
				sword->LOT.Drop = -WALL_SIZE * 20;
				sword->LOT.Fly = STEP_SIZE / 4;
				sword->LOT.Zone = ZONE_FLYER;
				CreatureAIInfo(item, &info);
			}
		}

		GetCreatureMood(item, &info, VIOLENT);
		CreatureMood(item, &info, VIOLENT);

		angle = CreatureTurn(item, sword->MaxTurn);

		if (item->ActiveState != 9)
			item->MeshBits = 0xFFFFFFFF;

		switch (item->ActiveState)
		{
		case 9:
			sword->MaxTurn = 0;

			if (!sword->Flags)
			{
				item->MeshBits = (item->MeshBits << 1) + 1;
				sword->Flags = 3;
			}
			else
			{
				sword->Flags--;
			}
			break;

		case 1:
			sword->MaxTurn = 0;

			if (info.ahead)
				head = info.angle;

			if (lara_alive)
			{
				if (info.bite && info.distance < 0x100000)
				{
					if (GetRandomControl() >= 0x4000)
						item->TargetState = 5;
					else
						item->TargetState = 3;
				}
				else
				{
					if (info.zoneNumber == info.enemyZone)
						item->TargetState = 2;
					else
						item->TargetState = 8;
				}
			}
			else
			{
				item->TargetState = 7;
			}
			break;

		case 2:
			sword->MaxTurn = ANGLE(9);

			if (info.ahead)
				head = info.angle;

			if (lara_alive)
			{
				if (info.bite && info.distance < 0x400000)
					item->TargetState = 10;
				else if (info.zoneNumber != info.enemyZone)
					item->TargetState = 1;
			}
			else
			{
				item->TargetState = 1;
			}
			break;

		case 3:
			sword->Flags = 0;

			if (info.ahead)
				torso = info.angle;

			if (!info.bite || info.distance > 0x100000)
				item->TargetState = 1;
			else
				item->TargetState = 4;
			break;

		case 5:
			sword->Flags = 0;

			if (info.ahead)
				torso = info.angle;

			if (!info.bite || info.distance > 0x100000)
				item->TargetState = 1;
			else
				item->TargetState = 6;
			break;

		case 10:
			sword->Flags = 0;

			if (info.ahead)
				torso = info.angle;

			if (!info.bite || info.distance > 0x400000)
				item->TargetState = 1;
			else
				item->TargetState = 11;
			break;

		case 8:
			sword->MaxTurn = ANGLE(7);

			if (info.ahead)
				head = info.angle;

			SwordGuardianFly(item);

			if (!sword->LOT.Fly)
				item->TargetState = 1;
			break;

		case 4:
		case 6:
		case 11:
			if (info.ahead)
				torso = info.angle;

			if (!sword->Flags && (item->TouchBits & 0xC000))
			{
				LaraItem->HitPoints -= 300;
				LaraItem->HitStatus = true;
				CreatureEffect(item, &swordBite, DoBloodSplat);
				sword->Flags = 1;
			}
			break;
		}
	}

	if (item->HitPoints > 0)
	{
		CreatureJoint(item, 0, torso);
		CreatureJoint(item, 1, head);
		CreatureAnimation(itemNum, angle, 0);
	}
}