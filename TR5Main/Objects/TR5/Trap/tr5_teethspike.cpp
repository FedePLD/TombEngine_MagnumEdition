#include "framework.h"
#include "tr5_teethspike.h"
#include "control.h"
#include "draw.h"
#include "lara.h"
#include "level.h"
#include "sound.h"
#include "tomb4fx.h"
#include "trmath.h"

short SPyoffs[8] =
{
	0xFC00, 0x0000, 0xFE00, 0x0000, 0x0000, 0x0000, 0xFE00, 0x0000
};

short SPxzoffs[8] =
{
	0x0000, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000, 0xFE00, 0x0000
};

short SPDETyoffs[8] =
{
	0x0400, 0x0200, 0x0200, 0x0200, 0x0000, 0x0200, 0x0200, 0x0200
};

void InitialiseTeethSpikes(short itemNumber)
{
	ITEM_INFO* item;
	int angle;

	item = &Items[itemNumber];
	item->status = ITEM_INVISIBLE;

	short rotations[8] =
	{
		-ANGLE(180.0f),
		-ANGLE(135.0f),
		-ANGLE(90.0f),
		-ANGLE(45.0f),
		ANGLE(0.0f),
		ANGLE(45.0f), 
		ANGLE(90.0f),
		ANGLE(135.0f)
	};
	
	if (item->triggerFlags & 8)
	{
		angle = item->triggerFlags & 7;
		item->pos.xRot = rotations[angle];
		item->pos.yRot = ANGLE(90.0f);
		item->pos.zPos -= SPxzoffs[angle];
	}
	else
	{
		angle = item->triggerFlags & 7;
		item->pos.zRot = rotations[angle];
		item->pos.xPos += SPxzoffs[angle];
	}

	item->itemFlags[0] = 1024;
	item->itemFlags[2] = 0;
	item->pos.yPos += SPyoffs[angle];
}

static int CollidedWithTeethSpikes(ITEM_INFO* item)
{
	short angle;
	int x;
	int z;

	if (item->triggerFlags & 8)
	{
		angle = item->triggerFlags & 7;
		x = item->pos.xPos & 0xFFFFFE00 | 0x200;
		z = (item->pos.zPos + SPxzoffs[angle]) & 0xFFFFFE00 | 0x200;
	}
	else
	{
		angle = item->triggerFlags & 7;
		x = (item->pos.xPos - SPxzoffs[angle]) & 0xFFFFFE00 | 0x200;
		z = item->pos.zPos & 0xFFFFFE00 | 0x200;
	}

	int delta = -((angle & 1) != 0);
	delta = delta & 0xFF4C;
	delta += 480;
	int y = item->pos.yPos + SPDETyoffs[angle];
	short* frames = GetBestFrame(LaraItem);

	if (LaraItem->pos.yPos + frames[2] <= y && LaraItem->pos.yPos + frames[3] >= y - 900)
	{
		if (LaraItem->pos.xPos + frames[0] <= (x + delta) && LaraItem->pos.xPos + frames[1] >= (x - delta))
		{
			if (LaraItem->pos.zPos + frames[4] <= (z + delta) && LaraItem->pos.zPos + frames[5] >= (z - delta))
				return 1;
		}
	}

	return 0;
}

void ControlTeethSpikes(short itemNumber)
{
	ITEM_INFO* item = &Items[itemNumber];

	if (!TriggerActive(item) || item->itemFlags[2])
	{
		if (TriggerActive(item))
		{
			item->itemFlags[1] -= item->itemFlags[0];
			item->itemFlags[0] += (item->itemFlags[0] >> 3) + 32;

			if (item->itemFlags[1] < 0)
			{
				item->itemFlags[0] = 1024;
				item->itemFlags[1] = 0;
				item->status = ITEM_INVISIBLE;
			}

			if (item->triggerFlags & 0x20)
			{
				item->itemFlags[2] = 1;
			}
			else
			{
				if (item->itemFlags[2])
				{
					item->itemFlags[2]--;
				}
			}
		}
		else if (!item->timer)
		{
			item->itemFlags[0] += (item->itemFlags[0] >> 3) + 32;

			if (item->itemFlags[1] > 0)
			{
				item->itemFlags[1] -= item->itemFlags[0];
				if (item->itemFlags[1] < 0)
					item->itemFlags[1] = 0;
			}
		}
	}
	else
	{
		if (item->itemFlags[0] == 1024)
			SoundEffect(SFX_TEETH_SPIKES, &item->pos, 0);

		item->status = ITEM_ACTIVE;

		if (LaraItem->hitPoints > 0 && CollidedWithTeethSpikes(item))
		{
			short* itemFrames = GetBestFrame(item);
			short* laraFrames = GetBestFrame(LaraItem);

			short angle = item->triggerFlags & 7;
			int numBloods = 0;

			if ((item->itemFlags[0] > 1024 || LaraItem->gravityStatus) && angle > 2 && angle < 6)
			{
				if (LaraItem->fallspeed > 6 || item->itemFlags[0] > 1024)
				{
					LaraItem->hitPoints = -1;
					numBloods = 20;
				}
			}
			else if (LaraItem->speed < 30)
			{
				numBloods = 0;
			}
			else
			{
				LaraItem->hitPoints -= 8;
				numBloods = (GetRandomControl() & 3) + 2;
			}

			int laraY1 = LaraItem->pos.yPos + laraFrames[2];
			int laraY2 = LaraItem->pos.yPos + laraFrames[3];

			short triggerFlags = item->triggerFlags & 0xF;
			int itemY1;
			int itemY2;

			if (triggerFlags != 8 && triggerFlags)
			{
				itemY1 = itemFrames[2];
				itemY2 = itemFrames[3];
			}
			else
			{
				itemY1 = -itemFrames[3];
				itemY2 = -itemFrames[2];
			}
			if (laraY1 < item->pos.yPos + itemY1)
				laraY1 = itemY1 + item->pos.yPos;
			if (laraY2 > item->pos.yPos + itemY2)
				laraY2 = itemY2 + item->pos.yPos;

			long dy = laraY1 - laraY2;
			int modulus = (HIDWORD(dy) ^ dy) - HIDWORD(dy) + 1;

			angle = item->triggerFlags & 7;
			if (angle == 2 || angle == 6)
				numBloods /= 2;

			for (int i = 0; i < numBloods; i++)
			{
				TriggerBlood(
					(GetRandomControl() & 0x7F) + LaraItem->pos.xPos - 64,
					laraY2 - GetRandomControl() % modulus,
					(GetRandomControl() & 0x7F) + LaraItem->pos.zPos - 64,
					2 * GetRandomControl(),
					1);
			}

			if (LaraItem->hitPoints <= 0)
			{
				short roomNumber = LaraItem->roomNumber;
				FLOOR_INFO* floor = GetFloor(LaraItem->pos.xPos, LaraItem->pos.yPos, LaraItem->pos.zPos, &roomNumber);
				int height = GetFloorHeight(floor, LaraItem->pos.xPos, LaraItem->pos.yPos, LaraItem->pos.zPos);

				if (item->pos.yPos >= LaraItem->pos.yPos && height - LaraItem->pos.yPos < 50)
				{
					LaraItem->animNumber = ANIMATION_LARA_SPIKED;
					LaraItem->frameNumber = Anims[LaraItem->animNumber].frameBase;
					LaraItem->currentAnimState = STATE_LARA_DEATH;
					LaraItem->goalAnimState = STATE_LARA_DEATH;
					LaraItem->gravityStatus = false;
				}
			}
		}

		item->itemFlags[0] += 128;
		item->itemFlags[1] += item->itemFlags[0];

		if (item->itemFlags[1] >= 5120)
		{
			item->itemFlags[1] = 5120;
			if (item->itemFlags[0] <= 1024)
			{
				item->itemFlags[0] = 0;
				if (!(item->triggerFlags & 0x10))
				{
					if (LaraItem->hitPoints > 0)
						item->itemFlags[2] = 64;
				}
			}
			else
			{
				item->itemFlags[0] = -item->itemFlags[0] >> 1;
			}
		}
	}
}