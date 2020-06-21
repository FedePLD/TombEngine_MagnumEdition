#include "framework.h"
#include "drip.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include "control.h"
#include "level.h"
#include "room.h"
#include "trmath.h"
#include "effect2.h"
namespace T5M {
	namespace Effects {
		namespace Drip {
			using namespace DirectX::SimpleMath;

			std::array<DripParticle, NUM_DRIPS> dripParticles;
			constexpr Vector4 DRIP_COLOR = Vector4(1, 1, 1, 1);
			void UpdateDrips()
			{
				for (int i = 0; i < dripParticles.size(); i++) {
					DripParticle& d = dripParticles[i];
					if (!d.active) continue;
					d.age++;
					if (d.age > d.life)
						d.active = false;
					d.velocity.y += d.gravity;
					if (Rooms[d.room].flags & ENV_FLAG_WIND) {
						d.velocity.x = SmokeWindX / 2;
						d.velocity.z = SmokeWindZ / 2;
					}
					d.pos += d.velocity;
					float normalizedAge = d.age / d.life;
					d.color = Vector4::Lerp(DRIP_COLOR, Vector4::Zero, normalizedAge);
					d.height = lerp(DRIP_WIDTH / 0.15625, 0, normalizedAge);
					short room = d.room;
					FLOOR_INFO* floor = GetFloor(d.pos.x, d.pos.y, d.pos.z, &room);
					int floorheight = floor->floor;
					int wh = GetWaterHeight(d.pos.x, d.pos.y, d.pos.z, d.room);
					if (d.pos.y > floorheight) {
						d.active = false;
					}
					if (d.pos.y > wh) {
						d.active = false;
						SetupRipple(d.pos.x, wh, d.pos.z, frandMinMax(16,24), RIPPLE_FLAG_SHORT_LIFE | RIPPLE_FLAG_RAND_ROT | RIPPLE_FLAG_LOW_OPACITY);
					}

				}
			}

			DripParticle& getFreeDrip()
			{
				for (int i = 0; i < dripParticles.size(); i++)
				{
					if (!dripParticles[i].active)
						return dripParticles[i];
				}
				return dripParticles[0];
			}

			void SpawnWetnessDrip(Vector3 pos, int room)
			{
				DripParticle& d = getFreeDrip();
				d = {};
				d.active = true;
				d.pos = pos;
				d.room = room;
				d.life = DRIP_LIFE;
				d.gravity = frandMinMax(3, 6);
			}

			void SpawnSplashDrips(Vector3& pos, int num,int room)
			{
				for (int i = 0; i < num; i++) {
					Vector3 dripPos = pos + Vector3(frandMinMax(-128, 128), frandMinMax(-128, 128), frandMinMax(-128, 128));
					Vector3 dir = (dripPos - pos);
					dir.Normalize();
					DripParticle& drip = getFreeDrip();
					drip = {};
					drip.pos = dripPos;
					drip.velocity = dir*16;
					drip.velocity -= Vector3(0, frandMinMax(32, 64), 0);
					drip.gravity = frandMinMax(3, 6);
					drip.room = room;
					drip.life = DRIP_LIFE_LONG;
					drip.active = true;
				}
			}

			void SpawnGunshellDrips(Vector3& pos, int room)
			{
				for (int i = 0; i < 4; i++) {
					Vector3 dripPos = pos + Vector3(frandMinMax(-16, 16), frandMinMax(-16, 16), frandMinMax(-16, 16));
					Vector3 dir = (dripPos - pos);
					dir.Normalize();
					DripParticle& drip = getFreeDrip();
					drip = {};
					drip.pos = dripPos;
					drip.velocity = dir * 16;
					drip.velocity -= Vector3(0, frandMinMax(16, 24), 0);
					drip.gravity = frandMinMax(2, 3);
					drip.room = room;
					drip.life = DRIP_LIFE_LONG;
					drip.active = true;
				}
			}

		}
	}
}