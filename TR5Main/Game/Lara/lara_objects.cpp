#include "framework.h"
#include "lara.h"
#include "input.h"
#include "sound.h"
#include "draw.h"
#include "rope.h"
#include "lara_tests.h"

/*This file has "all" lara_as/lara_col functions where Lara is interacting with an object.*/

/*pickups*/
void lara_as_pickup(ITEM_INFO* item, COLL_INFO* coll)//1AB00(<), 1AC34(<) (F)
{
	/*state 39, 98*/
	/*collision: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.targetAngle = -ANGLE(130.0f);
	Camera.targetElevation = -ANGLE(15.0f);
	Camera.targetDistance = SECTOR(1);
}

void lara_as_pickupflare(ITEM_INFO* item, COLL_INFO* coll)//1AB5C(<), 1AC90(<) (F)
{
	/*state 67*/
	/*collison: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.targetAngle = ANGLE(130.0f);
	Camera.targetElevation = -ANGLE(15.0f);
	Camera.targetDistance = SECTOR(1);
	if (item->frameNumber == g_Level.Anims[item->animNumber].frameEnd - 1)
		Lara.gunStatus = LG_NO_ARMS;
}
/*end pickups*/
/*-*/
/*switches*/
void lara_as_switchon(ITEM_INFO* item, COLL_INFO* coll)//1ABEC(<), 1AD20(<) (F)
{
	/*states 40, 126*/
	/*collision: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.targetAngle = ANGLE(80.0f);
	Camera.targetElevation = -ANGLE(25.0f);
	Camera.targetDistance = SECTOR(1);
	Camera.speed = 6;
}

void lara_as_switchoff(ITEM_INFO* item, COLL_INFO* coll)//1AC54(<), 1AD88(<) (F)
{
	/*state 41*/
	/*collision: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.targetAngle = ANGLE(80.0f);
	Camera.targetElevation = -ANGLE(25.0f);
	Camera.targetDistance = SECTOR(1);
	Camera.speed = 6;
}

void lara_col_turnswitch(ITEM_INFO* item, COLL_INFO* coll)//1B1B4(<), 1B2E8(<) (F)
{
	/*state 95*/
	/*state code: lara_as_controlledl*/
	if (coll->old.x != item->pos.xPos || coll->old.z != item->pos.zPos)
	{
		if (item->animNumber == LA_TURNSWITCH_PUSH_COUNTER_CLOCKWISE_CONTINUE)
		{
			item->pos.yRot -= ANGLE(90.0f);

			item->animNumber = LA_TURNSWITCH_PUSH_COUNTER_CLOCKWISE_END;
			item->frameNumber = g_Level.Anims[item->animNumber].frameBase;
		}

		if (item->animNumber == LA_TURNSWITCH_PUSH_CLOCKWISE_CONTINUE)
		{
			item->pos.yRot += ANGLE(90.0f);

			item->animNumber = LA_TURNSWITCH_PUSH_CLOCKWISE_END;
			item->frameNumber = g_Level.Anims[item->animNumber].frameBase;
		}
	}
}
/*end switches*/
/*-*/
/*puzzles and keys*/
void lara_as_usekey(ITEM_INFO* item, COLL_INFO* coll)//1ACBC(<), 1ADF0(<) (F)
{
	/*state 42*/
	/*collision: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.targetAngle = -ANGLE(80.0f);
	Camera.targetElevation = -4550;
	Camera.targetDistance = SECTOR(1);
}

void lara_as_usepuzzle(ITEM_INFO* item, COLL_INFO* coll)//1AD18(<), 1AE4C(<) (F)
{
	/*state 43*/
	/*collision: lara_default_col*/
	Lara.look = false;

	coll->enableBaddiePush = false;
	coll->enableSpaz = false;

	Camera.targetAngle = -ANGLE(80.0f);
	Camera.targetElevation = -ANGLE(25.0f);
	Camera.targetDistance = SECTOR(1);

	if (item->frameNumber == g_Level.Anims[item->animNumber].frameEnd)
	{
		if (item->itemFlags[0])
		{
			item->animNumber = item->itemFlags[0];
			item->currentAnimState = LS_MISC_CONTROL;
			item->frameNumber = g_Level.Anims[item->animNumber].frameBase;
		}
	}
}
/*end puzzles and keys*/
/*-*/
/*pushables*/
void lara_as_pushblock(ITEM_INFO* item, COLL_INFO* coll)//1AA04(<), 1AB38(<) (F)
{
	/*state 36*/
	/*collision: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.flags = CF_FOLLOW_CENTER;
	Camera.targetAngle = ANGLE(90.0f);
	Camera.targetElevation = -ANGLE(25.0f);
}

void lara_as_pullblock(ITEM_INFO* item, COLL_INFO* coll)//1AA60(<), 1AB94(<) (F)
{
	/*state 37*/
	/*collision: lara_default_col*/
	Lara.look = false;
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.flags = CF_FOLLOW_CENTER;
	Camera.targetAngle = ANGLE(35.0f);
	Camera.targetElevation = -ANGLE(25.0f);
}

void lara_as_ppready(ITEM_INFO* item, COLL_INFO* coll)//1AABC(<), 1ABF0(<) (F)
{
	/*state 38*/
	/*collision: lara_default_col*/
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	Camera.targetAngle = ANGLE(75.0f);
	if (!(TrInput & IN_ACTION))
		item->goalAnimState = LS_STOP;
}
/*end pushables*/
/*-*/
/*pulley*/
void lara_as_pulley(ITEM_INFO* item, COLL_INFO* coll)//1B288, 1B3BC (F)
{
	/*state 104*/
	/*collision: lara_default_col*/
	ITEM_INFO* p = (ITEM_INFO*)Lara.generalPtr;

	Lara.look = false;

	coll->enableSpaz = false;
	coll->enableBaddiePush = false;

	if (TrInput & IN_ACTION && p->triggerFlags)
	{
		item->goalAnimState = LS_PULLEY;
	}
	else
	{
		item->goalAnimState = LS_STOP;
	}

	if (item->animNumber == LA_PULLEY_PULL && item->frameNumber == g_Level.Anims[LA_PULLEY_PULL].frameBase + 44)
	{
		if (p->triggerFlags)
		{
			p->triggerFlags--;

			if (p->triggerFlags)
			{
				if (p->itemFlags[2])
				{
					p->itemFlags[2] = 0;
					p->status = ITEM_DEACTIVATED;
				}
			}
			else
			{
				if (!p->itemFlags[1])
					p->status = ITEM_DEACTIVATED;

				p->itemFlags[2] = 1;

				if (p->itemFlags[3] >= 0)
					p->triggerFlags = abs(p->itemFlags[3]);
				else
					p->itemFlags[0] = 1;
			}
		}
	}

	if (item->animNumber == LA_PULLEY_RELEASE && item->frameNumber == g_Level.Anims[item->animNumber].frameEnd - 1)
		Lara.gunStatus = LG_NO_ARMS;
}
/*end pulley*/
/*-*/
/*parallel bars*/
void lara_as_parallelbars(ITEM_INFO* item, COLL_INFO* coll)
{
	/*state 128*/
	/*collision: lara_default_col*/
	if (!(TrInput & IN_ACTION))
	{
		item->goalAnimState = LS_BARS_JUMP;
	}
}

void lara_as_pbleapoff(ITEM_INFO* item, COLL_INFO* coll)//1D244, 1D3D8 (F)
{
	/*state 129*/
	/*collision: lara_default_col*/
	ITEM_INFO* pitem = (ITEM_INFO*)Lara.generalPtr;

	item->gravityStatus = true;

	if (item->frameNumber == g_Level.Anims[item->animNumber].frameBase)
	{
		int dist;

		if (item->pos.yRot == pitem->pos.yRot)
		{
			dist = pitem->triggerFlags / 100 - 2;
		}
		else
		{
			dist = pitem->triggerFlags % 100 - 2;
		}

		item->fallspeed = -(20 * dist + 64);
		item->speed = 20 * dist + 58;
	}

	if (item->frameNumber == g_Level.Anims[item->animNumber].frameEnd)
	{
		item->pos.xPos += 700 * phd_sin(item->pos.yRot) >> W2V_SHIFT;
		item->pos.yPos -= 361;
		item->pos.zPos += 700 * phd_cos(item->pos.yRot) >> W2V_SHIFT;

		item->animNumber = LA_REACH;
		item->frameNumber = g_Level.Anims[item->animNumber].frameBase;

		item->goalAnimState = LS_REACH;
		item->currentAnimState = LS_REACH;
	}
}
/*end parallel bars*/
/*-*/
/*tightropes*/
void lara_as_trpose(ITEM_INFO* item, COLL_INFO* coll)
{
	/*state 119*/
	/*collision: lara_default_col*/
	if (TrInput & IN_LOOK)
		LookUpDown();

	GetTighRopeFallOff(127);
	if (LaraItem->currentAnimState != LS_TIGHTROPE_UNBALANCE_LEFT)
	{
		if (Lara.tightRopeFall)
		{
			if (GetRandomControl() & 1)
				item->goalAnimState = LS_TIGHTROPE_UNBALANCE_RIGHT;
			else
				item->goalAnimState = LS_TIGHTROPE_UNBALANCE_LEFT;
		}
		else
		{
			if (TrInput & IN_FORWARD)
			{
				item->goalAnimState = LS_TIGHTROPE_FORWARD;
			}
			else if ((TrInput & IN_ROLL) || (TrInput & IN_BACK))
			{
				if (item->animNumber == LA_TIGHTROPE_IDLE)
				{
					item->currentAnimState = LS_TIGHTROPE_TURN_180;
					item->animNumber = LA_TIGHTROPE_TURN_180;
					item->frameNumber = g_Level.Anims[item->animNumber].frameBase;
					GetTighRopeFallOff(1);
				}
			}
		}
	}
}

void lara_as_trwalk(ITEM_INFO* item, COLL_INFO* coll)
{
	/*state 121*/
	/*collision: lara_default_col*/
	if (Lara.tightRopeOnCount)
	{
		Lara.tightRopeOnCount--;
	}
	else if (Lara.tightRopeOff)
	{
		short roomNumber = item->roomNumber;

		if (GetFloorHeight(GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber),
			item->pos.xPos, item->pos.yPos, item->pos.zPos) == item->pos.yPos)
		{
			Lara.tightRopeOff = 0;
			item->goalAnimState = LS_TIGHTROPE_EXIT;
		}
	}
	else
	{
		GetTighRopeFallOff(127);
	}

	if (LaraItem->currentAnimState != LS_TIGHTROPE_UNBALANCE_LEFT)
	{
		if (TrInput & IN_LOOK)
		{
			LookUpDown();
		}

		if (item->goalAnimState != LS_TIGHTROPE_EXIT &&
			(Lara.tightRopeFall
				|| (TrInput & IN_BACK || TrInput & IN_ROLL || !(TrInput & IN_FORWARD)) && !Lara.tightRopeOnCount && !Lara.tightRopeOff))
		{
			item->goalAnimState = LS_TIGHTROPE_IDLE;
		}
	}
}

void lara_as_trfall(ITEM_INFO* item, COLL_INFO* coll)
{
	/*states 122, 123*/
	/*collision: lara_default_col*/
	if (item->animNumber == LA_TIGHTROPE_FALL_LEFT || item->animNumber == LA_TIGHTROPE_FALL_RIGHT)
	{
		if (item->frameNumber == g_Level.Anims[item->animNumber].frameEnd)
		{
			PHD_VECTOR pos;
			pos.x = 0;
			pos.y = 0;
			pos.z = 0;

			GetLaraJointPosition(&pos, LM_RFOOT);

			item->pos.xPos = pos.x;
			item->pos.yPos = pos.y + 75;
			item->pos.zPos = pos.z;

			item->goalAnimState = LS_FREEFALL;
			item->currentAnimState = LS_FREEFALL;

			item->animNumber = LA_FREEFALL;
			item->frameNumber = g_Level.Anims[item->animNumber].frameBase;

			item->fallspeed = 81;
			Camera.targetspeed = 16;
		}
	}
	else
	{
		int undoInp, wrongInput;
		int undoAnim, undoFrame;

		if (Lara.tightRopeOnCount > 0)
			Lara.tightRopeOnCount--;

		if (item->animNumber == LA_TIGHTROPE_UNBALANCE_LEFT)
		{
			undoInp = IN_RIGHT;
			wrongInput = IN_LEFT;
			undoAnim = LA_TIGHTROPE_RECOVER_LEFT;
		}
		else if (item->animNumber == LA_TIGHTROPE_UNBALANCE_RIGHT)
		{
			undoInp = IN_LEFT;
			wrongInput = IN_RIGHT;
			undoAnim = LA_TIGHTROPE_RECOVER_RIGHT;
		}
		else
		{
			return;
		}

		undoFrame = g_Level.Anims[item->animNumber].frameEnd + g_Level.Anims[undoAnim].frameBase - item->frameNumber;

		if (TrInput & undoInp && Lara.tightRopeOnCount == 0)
		{
			item->currentAnimState = LS_TIGHTROPE_RECOVER_BALANCE;
			item->goalAnimState = LS_TIGHTROPE_IDLE;
			item->animNumber = undoAnim;
			item->frameNumber = undoFrame;

			Lara.tightRopeFall--;
		}
		else
		{
			if (TrInput & wrongInput)
			{
				if (Lara.tightRopeOnCount < 10)
					Lara.tightRopeOnCount += (GetRandomControl() & 3) + 2;
			}
		}
	}
}
/*end tightropes*/
/*-*/
/*ropes*/
void lara_as_ropel(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	/*state 90*/
	/*collision: lara_void_func*/
	if (TrInput & IN_ACTION)
	{
		if (TrInput & IN_LEFT)
		{
			Lara.ropeY += 256;
		}
		else
		{
			item->goalAnimState = LS_ROPE_IDLE;
		}
	}
	else
	{
		FallFromRope(item);
	}
}

void lara_as_roper(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	if (TrInput & IN_ACTION)
	{
		if (TrInput & IN_RIGHT)
		{
			Lara.ropeY -= 256;
		}
		else
		{
			item->goalAnimState = LS_ROPE_IDLE;
		}
	}
	else
	{
		FallFromRope(item);
	}
}

void lara_as_rope(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	/*states 111, 114, 115*/
	/*collison: lara_col_rope(111), lara_col_ropefwd(114, 115)*/
	if (!(TrInput & IN_ACTION))
		FallFromRope(item);

	if (TrInput & IN_LOOK)
		LookUpDown();
}

void lara_col_rope(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	/*state: 111*/
	/*state code: lara_as_rope*/
	if (TrInput & IN_ACTION)
	{
		UpdateRopeSwing(item);

		if (TrInput & IN_SPRINT)
		{
			Lara.ropeDFrame = (g_Level.Anims[LA_ROPE_SWING].frameBase + 32) << 8;
			Lara.ropeFrame = Lara.ropeDFrame;

			item->goalAnimState = LS_ROPE_SWING;
		}
		else if (TrInput & IN_FORWARD && Lara.ropeSegment > 4)
		{
			item->goalAnimState = LS_ROPE_UP;
		}
		else if (TrInput & IN_BACK && Lara.ropeSegment < 21)
		{
			item->goalAnimState = LS_ROPE_DOWN;

			Lara.ropeFlag = 0;
			Lara.ropeCount = 0;
		}
		else if (TrInput & IN_LEFT)
		{
			item->goalAnimState = LS_ROPE_TURN_CLOCKWISE;
		}
		else if (TrInput & IN_RIGHT)
		{
			item->goalAnimState = LS_ROPE_TURN_COUNTER_CLOCKWISE;
		}
	}
	else
	{
		FallFromRope(item);
	}
}

void lara_col_ropefwd(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	/*states 114, 115*/
	/*state code: lara_as_rope(for both)*/
	Camera.targetDistance = SECTOR(2);

	UpdateRopeSwing(item);

	if (item->animNumber == LA_ROPE_SWING)
	{
		if (TrInput & IN_SPRINT)
		{
			int vel;

			if (abs(Lara.ropeLastX) < 9000)
				vel = 192 * (9000 - abs(Lara.ropeLastX)) / 9000;
			else
				vel = 0;

			ApplyVelocityToRope(Lara.ropeSegment - 2,
				item->pos.yRot + (Lara.ropeDirection ? ANGLE(0.0f) : ANGLE(180.0f)),
				vel >> 5);
		}

		if (Lara.ropeFrame > Lara.ropeDFrame)
		{
			Lara.ropeFrame -= (unsigned short)Lara.ropeFrameRate;
			if (Lara.ropeFrame < Lara.ropeDFrame)
				Lara.ropeFrame = Lara.ropeDFrame;
		}
		else if (Lara.ropeFrame < Lara.ropeDFrame)
		{
			Lara.ropeFrame += (unsigned short)Lara.ropeFrameRate;
			if (Lara.ropeFrame > Lara.ropeDFrame)
				Lara.ropeFrame = Lara.ropeDFrame;
		}

		item->frameNumber = Lara.ropeFrame >> 8;

		if (!(TrInput & IN_SPRINT) &&
			item->frameNumber == g_Level.Anims[LA_ROPE_SWING].frameBase + 32 &&
			Lara.ropeMaxXBackward < 6750 &&
			Lara.ropeMaxXForward < 6750)
		{
			item->animNumber = LA_JUMP_UP_TO_ROPE_END;
			item->frameNumber = g_Level.Anims[item->animNumber].frameBase;

			item->currentAnimState = LS_ROPE_IDLE;
			item->goalAnimState = LS_ROPE_IDLE;
		}

		if (TrInput & IN_JUMP)
			JumpOffRope(item);
	}
	else if (item->frameNumber == g_Level.Anims[LA_ROPE_IDLE_TO_SWING].frameBase + 15)
	{
		ApplyVelocityToRope(Lara.ropeSegment, item->pos.yRot, 128);
	}
}

void lara_as_climbrope(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	/*state 112*/
	/*collision: lara_void_func*/
	if (TrInput & IN_ROLL)
	{
		FallFromRope(item);
	}
	else
	{
		Camera.targetAngle = ANGLE(30.0f);

		if (g_Level.Anims[item->animNumber].frameEnd == item->frameNumber)
		{
			item->frameNumber = g_Level.Anims[item->animNumber].frameBase;
			Lara.ropeSegment -= 2;
		}

		if (!(TrInput & IN_FORWARD) || Lara.ropeSegment <= 4)
			item->goalAnimState = LS_ROPE_IDLE;
	}
}

void lara_as_climbroped(ITEM_INFO* item, COLL_INFO* coll) // (F) (D)
{
	/*state 113*/
	/*collision: lara_void_func*/
	LaraClimbRope(item, coll);
}
/*end ropes*/
/*-*/
/*poles*/
void lara_col_polestat(ITEM_INFO* item, COLL_INFO* coll)//16DFC, 16F30 (F)
{
	/*state 99*/
	/*state code: lara_as_null*/
	if (item->hitPoints <= 0)
	{
		item->goalAnimState = LS_FREEFALL;
		return;
	}

	coll->enableSpaz = false;
	coll->enableBaddiePush = false;

	if (item->animNumber == LA_POLE_IDLE)
	{
		coll->badPos = NO_BAD_POS;
		coll->badNeg = -STEPUP_HEIGHT;
		coll->badCeiling = BAD_JUMP_CEILING;

		Lara.moveAngle = 0;

		coll->facing = Lara.moveAngle;
		coll->radius = 100;
		coll->slopesAreWalls = true;

		GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HITE);

		if (TrInput & IN_ACTION)
		{
			item->goalAnimState = LS_POLE_IDLE;

			if (TrInput & IN_LEFT)
			{
				item->goalAnimState = LS_POLE_TURN_CLOCKWISE;
			}
			else if (TrInput & IN_RIGHT)
			{
				item->goalAnimState = LS_POLE_TURN_COUNTER_CLOCKWISE;
			}

			if (TrInput & IN_LOOK)
				LookUpDown();

			if (TrInput & IN_FORWARD)
			{
				short roomNum = item->roomNumber;

				if (item->pos.yPos - GetCeiling(GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNum),
					item->pos.xPos, item->pos.yPos, item->pos.zPos) > SECTOR(1))
				{
					item->goalAnimState = LS_POLE_UP;
				}
			}
			else if (TrInput & IN_BACK && coll->midFloor > 0)
			{
				item->goalAnimState = LS_POLE_DOWN;
				item->itemFlags[2] = 0;
			}

			if (TrInput & IN_JUMP)
				item->goalAnimState = LS_JUMP_BACK;
		}
		else if (coll->midFloor <= 0)
		{
			item->goalAnimState = LS_STOP;
		}
		else
		{
			item->pos.xPos -= (phd_sin(item->pos.yRot)) << 6 >> W2V_SHIFT;
			item->pos.zPos -= (phd_cos(item->pos.yRot)) << 6 >> W2V_SHIFT;
			item->goalAnimState = LS_FREEFALL;
		}
	}
}

void lara_col_poleup(ITEM_INFO* item, COLL_INFO* coll)//170D8(<), 1720C(<) (F)
{
	/*state: 100*/
	/*state code: lara_as_null*/
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;

	if (TrInput & IN_LOOK)
		LookUpDown();

	if (!(TrInput & IN_ACTION) || !(TrInput & IN_FORWARD) || item->hitPoints <= 0)
		item->goalAnimState = LS_POLE_IDLE;

	short roomNumber = item->roomNumber;

	if (item->pos.yPos -
		GetCeiling(GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber),
			item->pos.xPos, item->pos.yPos, item->pos.zPos) < SECTOR(1))
		item->goalAnimState = LS_POLE_IDLE;
}

void lara_col_poledown(ITEM_INFO* item, COLL_INFO* coll)//171A0, 172D4 (F)
{
	/*state: 101*/
	/*state code: lara_as_null*/
	coll->enableSpaz = false;
	coll->enableBaddiePush = false;

	if (TrInput & IN_LOOK)
		LookUpDown();

	if ((TrInput & (IN_BACK | IN_ACTION)) != (IN_BACK | IN_ACTION) || item->hitPoints <= 0)
		item->goalAnimState = LS_POLE_IDLE;

	coll->badPos = NO_BAD_POS;
	coll->badNeg = -STEPUP_HEIGHT;
	coll->badCeiling = 0;

	Lara.moveAngle = 0;

	coll->slopesAreWalls = true;

	coll->facing = Lara.moveAngle;
	coll->radius = 100;

	coll->facing = Lara.moveAngle;
	GetCollisionInfo(coll, item->pos.xPos, item->pos.yPos, item->pos.zPos, item->roomNumber, LARA_HITE);

	if (coll->midFloor < 0)
	{
		short roomNumber = item->roomNumber;
		item->floor = GetFloorHeight(GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber),
			item->pos.xPos, item->pos.yPos - 762, item->pos.zPos);

		item->goalAnimState = LS_POLE_IDLE;
		item->itemFlags[2] = 0;
	}

	if (TrInput & IN_LEFT)
	{
		item->pos.yRot += 256;
	}
	else if (TrInput & IN_RIGHT)
	{
		item->pos.yRot -= 256;
	}

	if (item->animNumber == LA_POLE_DOWN_END)
	{
		item->itemFlags[2] -= SECTOR(1);
	}
	else
	{
		item->itemFlags[2] += 256;
	}

	// CHECK
	SoundEffect(SFX_LARA_ROPEDOWN_LOOP, &item->pos, 0);

	item->itemFlags[2] = CLAMP(item->itemFlags[2], 0, ANGLE(90.0f));

	item->pos.yPos += item->itemFlags[2] >> 8;
}

void lara_as_poleleft(ITEM_INFO* item, COLL_INFO* coll)//17020(<), 17154(<) (F)
{
	/*state 102*/
	/*collision: lara_void_func*/
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	if (!(TrInput & IN_LEFT) || !(TrInput & IN_ACTION) || (TrInput & (IN_FORWARD | IN_BACK)) || item->hitPoints <= 0)
		item->goalAnimState = LS_POLE_IDLE;
	else
		item->pos.yRot += 256;
}

void lara_as_poleright(ITEM_INFO* item, COLL_INFO* coll)//1707C(<), 171B0(<) (F)
{
	/*state: 103*/
	/*collision: lara_void_func*/
	coll->enableBaddiePush = false;
	coll->enableSpaz = false;
	if (!(TrInput & IN_RIGHT) || !(TrInput & IN_ACTION) || (TrInput & (IN_FORWARD | IN_BACK)) || item->hitPoints <= 0)
		item->goalAnimState = LS_POLE_IDLE;
	else
		item->pos.yRot -= 256;
}
/*end poles*/
/*-*/
/*deathslide*/
void lara_as_deathslide(ITEM_INFO* item, COLL_INFO* coll)//1B038, 1B16C (F)
{
	/*state 70*/
	/*collision: lara_void_func*/
	short roomNumber = item->roomNumber;

	Camera.targetAngle = ANGLE(70.0f);

	GetFloorHeight(GetFloor(item->pos.xPos, item->pos.yPos, item->pos.zPos, &roomNumber),
		item->pos.xPos, item->pos.yPos, item->pos.zPos);

	coll->trigger = TriggerIndex;

	if (!(TrInput & IN_ACTION))
	{
		item->goalAnimState = LS_JUMP_FORWARD;

		AnimateLara(item);

		LaraItem->gravityStatus = true;
		LaraItem->speed = 100;
		LaraItem->fallspeed = 40;

		Lara.moveAngle = 0;
	}
}
/*end deathslide*/