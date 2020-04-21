#pragma once
#include "Timer.h"
#include "Vector2.h"
#include <vector>
#include "Weapon.h"
class Tile;
namespace  EnemyDataStructs 
{
	struct MovementStruct {
		bool Moved = false;
		bool RoomBound = false;
		bool CanMove = true;
		Timer MoveTimerSec = Timer(1.0f);
		void Print() 
		{
			printf("MovementData: Moved %s, RoomBound %s, CanMove %s, MoveRate (TPS) %f\n", (Moved ? "true": "false"), (RoomBound ? "true": "false"), (CanMove ? "true": "false"), MoveTimerSec.GetResetTime());
		}
		

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
		void Print()
		{
			printf("ChargetData: DoesDamage %s, Charging %s, StopAtTarget %s, ChargeDamage %d, ChargeMovementIntervals (TPS) %f, Wind up %f, Charge Chance %d, Charge Frequency %f, Minimum distance (tiles) %d\n", (DoesDamage ? "true": "false"), (Charging ? "true": "false"), (StopAtTarget ? "true": "false"), ChargeDamage, ChargeMovementIntervals.GetResetTime(), ChargeWindUpSec.GetResetTime(), ChanceToCarge, ShouldChargeCheckIntervals.GetResetTime(), MinDistance);
		}
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
		void Print()
		{
			/*
			printf("Connst Times %s, Invisible Time %f, min max invisible %s, visible Time %f, min max Visible %s, flicker Time %f, min max Flicker %s, Flicker Rate"
				, (ConstTimes ? "true" : "false"),
				TimeInvisible.GetResetTime(), MinMaxInvisibleTime.toString().c_str(),
				TimeVisible.GetResetTime(), MinMaxVisibleTime.toString().c_str(),
				TimeFlickering.GetResetTime(), MinMaxFlickerTime.toString().c_str()),
				FlickerRate.GetResetTime());*/
		}

	};


	struct BehaviorStruct
	{
		

		//patrol variables
		std::vector<int>  PartolPointsAsIndexes;
		int currentIndex = 0;
		int GetTarget() {
			return PartolPointsAsIndexes[currentIndex];
		}
		void IncrementIndex() 
		{
			currentIndex++;
			if (currentIndex > PartolPointsAsIndexes.size()-1)
			{
				currentIndex = 0;
			}
		}
		bool UseDiagonals = false;
		std::vector<int> Path;
		
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
		bool Dies = true;
		Vector2 BarrierLifeTimeRange = Vector2();
		Timer PlaceBlockInterval = Timer(1.5f);
		Timer TimeToDropBlock = Timer(0.5f);
		Vector2 PlaceBlockTimeRange = Vector2();
		int BlockDropChance = 100;
	};

	struct DetectionStruct
	{
		int DetectionRadius = 5;//in tiles
		int DetectionRadiusLost = 15; //in tiles how far before they deaggro
		bool Detected = false;
	};

	struct SummonStruct {
		Timer SummonIntervals = Timer(0.5f); //how frequently a summon attempt occurs
		Vector2 SummonIntervalsRange = Vector2(3, 3);
		Timer SummonTime = Timer(0.5f); //how long it takes to summon
		Vector2 SummonTimeRange = Vector2(0.1f, 0.1f);
		int SummonRange = 1;
		int SummonChance = 100; // chance for summon attempt to succeeed
		Tile* SpawnTarget = nullptr;
	
	};
	struct MeleeStruct{
		int Range = 1;
		int Damage = 2;
		Timer AttackFrequency = Timer(1.0f);
	};
	struct RangedStruct{
		Weapon* Weapon = nullptr;
	};
};


