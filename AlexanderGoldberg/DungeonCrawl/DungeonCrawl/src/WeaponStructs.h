#pragma once
#include "Timer.h"
#include "Vector2.h"
#include "Thing.h"
namespace WeaponStructs 
{

	struct BurstStruct {
		bool burstFireing = false;
		Timer burstTimer = Timer(0.5f);
		float burstTotalTime = 0.75f; //the total amount of time that the burst shot should last it will be divided by the number of shots fired for the trigger pull (m_burstShotsForCurrentBurst, which is determined depending on if the burst is set ot constant or variable)
		int burstShotsRemaining = -1;
		int burstShotsForCurrentBurst = -1;
		Vector2 burstShotsVariable = Vector2(1, 3); // the number of shots fired per burst, in the case of it being set to burstConstant the X value is used
		void PrintData() 
		{
			
			printf("burstShotsRemaining is %d, burstShotsVariable is %s, burstShotsForCurrentBurst is %d, burstTotalTime %f\n",burstShotsRemaining, burstShotsVariable.toString().c_str(), burstShotsForCurrentBurst, burstTotalTime);
		}

	};
	struct ProjectileStruct {
		
		float projectileSpeedMultiplier = 1;
		int knockBackMultiplier = 0;
		int recoilMultiplier = 0;
		int piercingCount = 1;
		int DamageAmount = 5;
		Thing* LastThingHit = nullptr;
		void PrintData()
		{

			printf("DamageAmount is %d, piercingCount is %d\n", DamageAmount, piercingCount);
		}
	};
	struct LightStruct {

		int lightRadius = 1;
		int lightIntensity = 0;
		void PrintData()
		{

			printf("lightRadius is %d, lightIntensity is %d\n", lightRadius, lightIntensity);
		}
	};
	struct SpreadStruct {

		Vector2 spreadRangeDeg = Vector2(-15, 15); //in degrees
		std::vector<int> constSpreadInDegreesPerProjectile;
		void PrintData()
		{

			printf("spread range is %s.", spreadRangeDeg.toString().c_str());

			if (constSpreadInDegreesPerProjectile.size() > 0)
			{
				printf("constSpreadInDegreesPerProjectile values are;");
				for (size_t i = 0; i < constSpreadInDegreesPerProjectile.size(); i++)
				{
					printf("%d", constSpreadInDegreesPerProjectile[i]);
					if (i < constSpreadInDegreesPerProjectile.size() - 1)
					{
						printf(", ");
					}
				}
				printf("\n");
			}
			}
			
	};
	struct TriggerPullStruct {

		float fireRate = 1.0f; //delay between trigger pulls.  If this weapons an burst fire this should be greater than the number of shots per burst*the delay between each shot
		Timer fireTimer = Timer(1.0f); //timer for delay between trigger pulls
		int shotsFiredperTriggerPull = 1; //the number of projectiles spawned when per fire command, if the weapon is set to burst this is the number of prokectiles per burst set  as a result the total number of prokectiles fired is equal to m_burstShotsForCurrentBurst * m_shotsFiredperTriggerPull
		void PrintData()
		{

			printf("fireRate is %f, shotsFiredperTriggerPull is %d\n", fireRate, shotsFiredperTriggerPull);
		}
	};
	struct AOEStruct {

		int AoeRadiusInTiles = 1;
		float AoePercentageFallOffPerUnitDistance = 0.5f;
		void PrintData()
		{

			printf("AoeRadiusInTiles is %d, AoePercentageFallOffPerUnitDistance is %d\n", AoeRadiusInTiles, AoePercentageFallOffPerUnitDistance);
		}
	};
	struct BounceStruct {
		int bounceMax = 2;
		int thisBounce = 1;
		Thing* LastThingHit[2] = { nullptr ,nullptr };
		Timer bounceCoolDown = Timer(0.5f);
		void PrintData()
		{

			printf("bounceMax is %d, thisBounce is %d\n", bounceMax, thisBounce);
		}
		bool RecentlyHit(Thing* _other) 
		{
			for (size_t i = 0; i < LHTSize; i++)
			{
				if (LastThingHit[i] == _other)
				{
					return true;
				}
			}
			return false;
		}
		int Index() {
			LHTIndex++;
			if (LHTIndex > LHTSize - 1)
			{
				LHTIndex = 0;
				return LHTSize - 1;
			}
			return LHTIndex - 1;
		}
		void ClearLastThingHit() 
		{
			for (size_t i = 0; i < LHTSize; i++)
			{
				LastThingHit[i] = nullptr;
			}
		}
	private:
		int LHTIndex = 0;
		int LHTSize = 2;
	};
	
	

}