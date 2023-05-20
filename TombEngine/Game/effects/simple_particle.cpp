#include "framework.h"
#include "Game/effects/simple_particle.h"

#include "Game/items.h"
#include "Game/Setup.h"
#include "Math/Math.h"

using namespace TEN::Math;

namespace TEN::Effects
{
	std::array<SimpleParticle, 15> simpleParticles;

	SimpleParticle& GetFreeSimpleParticle()
	{
		for (auto& p : simpleParticles)
			if (!p.active)
				return p;

		return simpleParticles[0];
	}

	void TriggerSnowmobileSnow(ItemInfo* snowMobile)
	{
		float angle = TO_RAD(snowMobile->Pose.Orientation.y);
		const float angleVariation = Random::GenerateFloat(-10, 10) * RADIAN;
		float x = std::sin(angle + angleVariation);
		float z = std::cos(angle + angleVariation);
		x = x* -500 + snowMobile->Pose.Position.x;
		z = z* -500 + snowMobile->Pose.Position.z;
		SimpleParticle& p = GetFreeSimpleParticle();
		p = {};
		p.active = true;
		p.life = Random::GenerateFloat(8, 14);
		p.room = snowMobile->RoomNumber;
		p.ageRate = Random::GenerateFloat(0.9f, 1.3f);
		float size = Random::GenerateFloat(96, 128);
		p.worldPosition = {x, float(snowMobile->Pose.Position.y) - size / 2 , z};
		p.sequence = ID_SKIDOO_SNOW_TRAIL_SPRITES;
		p.size = Random::GenerateFloat(256, 512);
		p.blendMode = BLENDMODE_ALPHABLEND;
	}

	void TriggerSpeedboatFoam(ItemInfo* boat, Vector3 offset)
	{
		for (float i = -0.5; i < 1; i += 1)
		{
			float size = Random::GenerateFloat(96, 128);
			float angle = TO_RAD(boat->Pose.Orientation.y);
			float angleVariation = i*2*10 * RADIAN;
			float y = float(boat->Pose.Position.y) - size / 2 + offset.y;
			float x = std::sin(angle + angleVariation);
			float z = std::cos(angle + angleVariation);
			x = x * offset.z + z * offset.x + boat->Pose.Position.x;
			z = z * offset.z + x * offset.x + boat->Pose.Position.z;
			SimpleParticle& p = GetFreeSimpleParticle();
			p = {};
			p.active = true;
			p.life = Random::GenerateFloat(5, 9);
			p.room = boat->RoomNumber;
			p.ageRate = Random::GenerateFloat(0.9f, 1.3f);
			p.worldPosition = { x, y, z };
			p.sequence = ID_MOTORBOAT_FOAM_SPRITES;
			p.size = Random::GenerateFloat(256, 512);
			p.blendMode = BLENDMODE_ADDITIVE;
		}
	}

	void UpdateSimpleParticles()
	{
		for (auto& p : simpleParticles)
		{
			if (!p.active)
				continue;

			p.age+= p.ageRate;
			if (p.life < p.age)
				p.active = false;

			int numSprites = -Objects[p.sequence].nmeshes - 1;
			float normalizedAge = p.age / p.life;
			p.sprite = Lerp(0.0f, numSprites, normalizedAge);
		}
	}
}
