#pragma once
#include "Timer.h"
#include "Vector2.h"
#include <vector>
namespace  EnemyDataStructs 
{
	struct MovementStruct {
		bool Moved = false;
		bool RoomBound = false;
		bool CanMove = true;
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
		Timer TimeOff = Timer(1.0f);// Timer for how long the timed shield should be on
		Timer TimeOn = Timer(1.0f);	// Timer for how long the timed shield should be off
		bool VariableTimes = false;
		Vector2 MinMaxTimeOff = Vector2(1, 1);
		Vector2 MinMaxTimeOn = Vector2(1, 1);

		//breakable shield
		bool Regens = false; //should the shield regenerate
		Timer RechargeTimerSec = Timer(1.0f); //timer for shield regeneration
		int RechargeAmount = 1; //amount to recharge per recharge increment
		int ShieldMax = 10; //max shield strength
		int ShieldCurrent = 10; // amount of shield strength left, for timed shields > 0 is on <= 0 is off
		void Recharge() 
		{
			ShieldCurrent += RechargeAmount;
			if (ShieldCurrent > ShieldMax)
				ShieldCurrent = ShieldMax;
		}
	};
	struct VisibilityStruct
	{
		//Flicker
		enum class VisibleStatus{
			Visible,
			Invisible,
			Flickering
		};
		bool ConstTimes = true;
		VisibleStatus Status = VisibleStatus::Visible;
		VisibleStatus LastStatus = VisibleStatus::Visible;	
		Vector2 MinMaxInvisibleTime = Vector2(2, 2); //for the time to remain invisible if const times are false picks a random floating point number (of precision RandomPrecision) between teh X and Y value
		Vector2 MinMaxVisibleTime = Vector2(2, 2);//for the time to remain visible if const times are false picks a random floating point number (of precision RandomPrecision) between teh X and Y value
		Vector2 MinMaxFlickerTime = Vector2(2, 2);//for the time to flickering invisible if const times are false picks a random floating point number (of precision RandomPrecision) between teh X and Y value
		Timer TimeInvisible = Timer(1.0f); 
		Timer TimeVisible = Timer(1.0f);
		Timer TimeFlickering = Timer(1.0f);
		Timer FlickerRate = Timer(0.05f); //flicker rate while flickering

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
		bool Aggroded = false;
	};


	struct MineLayerStruct
	{
		Timer MineDropFrequency = Timer(2.0f); //how frequently to drop mines
		Timer TimeToDropMine = Timer(0.02f);
		int MineDropChance = 100;
		int MineDamage = 3;
		int MineCharges = 3;
		std::vector<Thing*> AvailableMines;
		void AddMine(Thing* _newMine) 
		{
			AvailableMines.push_back(_newMine);
		}
		Thing* GetMine() 
		{
			for (size_t i = 0; i < AvailableMines.size(); i++)
			{
				if (!AvailableMines[i]->ShouldCheckCollision())
				{
					return AvailableMines[i];
				}
			}
			return nullptr;
		}

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


