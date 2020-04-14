#pragma once
#include "Timer.h"
#include "Vector2.h"
#include <vector>
namespace  EnemyDataStructs 
{
	struct MovementStruct {
		bool Moved = false;
		bool RoomBound = false;
		Timer MoveTimerSec = Timer(1.0f);
		
		

	};

	struct ChargeStruct {

		//charge variables
		int MinDistance = 4; // enemy must be at least this far away to charge
		int ChargeTargetIndex = -1; // the tile to charge toward, set at beginning of charge
		Timer ShouldChargeCheckIntervals = Timer(1.0f); // how frequently the enemy checks if it should charg
		int ChanceToCarge = 100; //the chance to charge when it checks
		Timer ChargeWindUpSec = Timer(1.0f); // how long the wind up is before charging
		Timer ChargeMovementIntervals = Timer(0.1f); //the rate the enemy moves while charging
		bool DoesDamage = false;
		bool Charging = false; // if the enemy is currently chargin
		bool StopAtTarget = false; //should the enemy stop when it reaches its target
		bool StunnedOnImpact = false; // is the enemy stunned when it runs into something
		int ChargeDamage = 5; //how much damage does it do to the player if they are adjacent when it stops charging
		int DirectionOfCharge = -1; //the direction the enemy is charging in, set when charge begins
		
	};
	struct ShieldStruct 
	{
		//timer shield
		Timer TimeOff = Timer(1.0f);
		Timer TimeOn = Timer(1.0f);
		float TimeShieldOn = 1.0f;
		float TimeShieldOff = 1.0f;

		//breakable shield
		bool Regens = false;
		Timer RechargeTimerSec = Timer(1.0f);
		float RechargeRate = 1.0f;
		int RechargeAmount = 1;
		int ShieldMax = 10;
		int ShieldCurrent = 10;
	};

	struct HealthStruct {
		int HealthRegenRate = 1;
		int HealthMax = 100;
		int Health = 100;
	};
	struct VisibilityStruct
	{

	};


	struct BehaviorStruct
	{
		

		//patrol variables
		std::vector<int>  PartolPointsAsIndexes;
		int currentIndex = 0;
		void IncrementIndex() 
		{
			currentIndex++;
			if (currentIndex > PartolPointsAsIndexes.size() -1)
			{
				currentIndex = 0;
			}
		}
		bool UseDiagonals = false;

		
		//seek out variables
		bool pursueBeyondRoom = false;

		//keepdistance variables
		Vector2 MinMaxDist = Vector2(1,1);

		
	};


	struct ContactStruct
	{

	};


	struct MineLayerStruct
	{

	};


	struct DodgeStruct
	{

	};


	struct BarrierStruct
	{

	};

	struct DetectionStruct
	{
		int DetectionRadius = 5;//in tiles
		int DetectionRadiusLost = 15; //in tiles how far before they deaggro
		bool Detected = false;
	};
};


