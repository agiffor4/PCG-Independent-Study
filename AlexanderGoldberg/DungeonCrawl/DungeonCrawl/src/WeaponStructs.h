#pragma once
#include "Timer.h"
#include "Vector2.h"
namespace WeaponStructs 
{

	struct BurstStruct {
		bool burstFireing = false;
		Timer burstTimer = Timer(0.5f);
		float burstTotalTime = 2.0f; //the total amount of time that the burst shot should last it will be divided by the number of shots fired for the trigger pull (m_burstShotsForCurrentBurst, which is determined depending on if the burst is set ot constant or variable)
		int burstShotsRemaining = -1;
		int burstShotsForCurrentBurst = -1;
		Vector2 burstShotsVariable = Vector2(3, 5); // the number of shots fired per burst, in the case of it being set to burstConstant the X value is used

	};
	struct ProjectileStruct {
		
		float projectileSpeedMultiplier = 1;
		int knockBackMultiplier = 0;
		int recoilMultiplier = 0;
	};
	struct LightStruct {

		int lightRadius = 0;
		int lightIntensity = 0;
	};
	struct SpreadStruct {

		Vector2 spreadRangeDeg = Vector2(-15, 15); //in degrees
		std::vector<int> constSpreadInDegreesPerProjectile;
	};
	struct TriggerPullStruct {

		float fireRate = 1.0f; //delay between trigger pulls.  If this weapons uses burst fire this should be greater than the number of shots per burst*the delay between each shot
		Timer fireTimer = Timer(1.0f); //timer for delay between trigger pulls
		int shotsFiredperTriggerPull = 1; //the number of projectiles spawned when per fire command, if the weapon is set to burst this is the number of prokectiles per burst set  as a result the total number of prokectiles fired is equal to m_burstShotsForCurrentBurst * m_shotsFiredperTriggerPull
	};
	struct AOEStruct {

		int AoeRadiusInTiles = 1;
		float AoePercentageFallOffPerUnitDistance = 0.5f;
	};
	
	

}