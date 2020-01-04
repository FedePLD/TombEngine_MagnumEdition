#include "healt.h"
#include "draw.h"
#include "pickup.h"
#include "lara.h"

short PickupX;
short PickupY;
short CurrentPickup;
DISPLAY_PICKUP Pickups[NUM_PICKUPS];
short PickupVel;
int OldHitPoints = 1000;
int HealtBarTimer = 40;
int FlashState = 0;
int FlashCount = 0;
int PoisonFlag = 0;
int DashTimer = 0;

extern LaraExtraInfo g_LaraExtra;

void DrawHealthBarOverlay(int value)
{
	if (CurrentLevel)
	{
		int color2 = 0;
		if (Lara.poisoned || Lara.gassed)
			color2 = 0xA0A000;
		else
			color2 = 0xA00000;
		g_Renderer->DrawBar(245, 32, 150, 12, value, 0xA00000, color2);
	}
}

void DrawHealthBar(int value)
{
	if (CurrentLevel)
	{
		int color2;
		if (Lara.poisoned || Lara.gassed)
			color2 = 0xA0A000;
		else
			color2 = 0xA00000;
		g_Renderer->DrawBar(20, 32, 150, 12, value, 0xA00000, color2);
	}
}

void UpdateHealtBar(int flash)
{
	int hitPoints = LaraItem->hitPoints;

	if (hitPoints < 0)
		hitPoints = 0;
	else if (hitPoints > 1000)
		hitPoints = 1000;

	if (OldHitPoints != hitPoints)
	{
		OldHitPoints = hitPoints;
		HealtBarTimer = 40;
	}

	if (HealtBarTimer < 0)
		HealtBarTimer = 0;

	if (hitPoints <= 1000 / 4)
	{
		if (!BinocularRange)
		{
			if (flash)
				DrawHealthBar(hitPoints / 10);
			else
				DrawHealthBar(0);
		}
		else 
		{
			if (flash)
				DrawHealthBarOverlay(hitPoints / 10);
			else
				DrawHealthBarOverlay(0);
		}
	}
	else if ((HealtBarTimer > 0)
		|| (hitPoints <= 0)
		|| (Lara.gunStatus == LG_READY && Lara.gunType != WEAPON_TORCH)
		|| (Lara.poisoned >= 256))
	{
		if (!BinocularRange && !SniperOverlay)
		{
			DrawHealthBar(hitPoints / 10);
		}
		else
		{
			DrawHealthBarOverlay(hitPoints / 10);
		}
	}

	if (PoisonFlag)
		PoisonFlag--;
}

void DrawAirBar(int value)
{
	if (CurrentLevel)
	{
		g_Renderer->DrawBar(20, 10, 150, 12, value, 0x0000A0, 0x0050A0);
	}
}

void UpdateAirBar(int flash)
{
	if (Lara.air == 1800 || LaraItem->hitPoints <= 0)
		return;

	if ((g_LaraExtra.Vehicle == NO_ITEM)
		|| (Items[g_LaraExtra.Vehicle].objectNumber != ID_UPV))
	{
		if ((Lara.waterStatus != LW_UNDERWATER)
			&& (Lara.waterStatus != LW_SURFACE)
			&& (!((Rooms[LaraItem->roomNumber].flags & ENV_FLAG_SWAMP)
				&& (Lara.waterSurfaceDist < -775))))
			return;
	}

	int air = Lara.air;
	if (air < 0)
		air = 0;
	else if (air > 1800)
		air = 1800;
	if (air <= 450)
	{
		if (flash)
			DrawAirBar((air * 100) / 1800);
		else
			DrawAirBar(0);
	}
	else
		DrawAirBar((air * 100) / 1800);

	if (Lara.gassed)
	{
		if (Lara.dpoisoned < 2048)
			Lara.dpoisoned += 2;
		Lara.gassed = false;
	}
}

void DrawDashBar(int value)
{
	if (CurrentLevel)
	{
		g_Renderer->DrawBar(630, 32, 150, 12, value, 0xA0A000, 0x00A000);
	}
}

int DrawAllPickups()
{
	if (Pickups[CurrentPickup].life > 0)
	{
		if (PickupX > 0)
		{
			PickupX += -PickupX >> 5;
			return g_Renderer->DrawPickup(Pickups[CurrentPickup].objectNumber);
		}
		else
		{
			Pickups[CurrentPickup].life--;
			return g_Renderer->DrawPickup(Pickups[CurrentPickup].objectNumber);
		}
	}
	else if (Pickups[CurrentPickup].life == 0)
	{
		if (PickupX < 128)
		{
			if (PickupVel < 16)
				PickupVel++;
			PickupX += PickupVel >> 2;
			return g_Renderer->DrawPickup(Pickups[CurrentPickup].objectNumber);
		}
		else
		{
			Pickups[CurrentPickup].life = -1;
			PickupVel = 0;
			return g_Renderer->DrawPickup(Pickups[CurrentPickup].objectNumber);
		}
	}

	int pickupIndex = CurrentPickup;
	int i = 0;
	do
	{
		if (Pickups[pickupIndex].life > 0)
			break;
		pickupIndex = (pickupIndex + 1) & 7;
		i++;
	} while (i < 8);

	CurrentPickup = pickupIndex;
	if (i != 8)
		return g_Renderer->DrawPickup(Pickups[CurrentPickup].objectNumber);

	CurrentPickup = 0;

	return 0;
}


void AddDisplayPickup(short objectNumber)
{
	for (int i = 0; i < 8; i++)
	{
		DISPLAY_PICKUP* pickup = &Pickups[i];
		if (pickup->life < 0)
		{
			pickup->life = 45;
			pickup->objectNumber = objectNumber;
			PickedUpObject(objectNumber);
			return;
		}
	}

	// No free slot found, so just pickup the object ithout displaying it
	PickedUpObject(objectNumber);
}

void InitialisePickupDisplay()
{
	for (int i = 0; i < 8; i++)
	{
		DISPLAY_PICKUP* pickup = &Pickups[i];
		pickup->life = -1;
	}

	PickupX = 128;
	PickupY = 128;
	PickupVel = 0;
	CurrentPickup = 0;
}

int FlashIt()
{
	if (FlashCount)
		FlashCount--;
	else
	{
		FlashState ^= 1;
		FlashCount = 5;
	}
	return FlashState;
}

void Inject_Healt()
{

}
