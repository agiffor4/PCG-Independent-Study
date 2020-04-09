#pragma once
namespace  EnemyDataStructs 
{
	struct MovementStruct {
		bool RoomBound = false;


	};
	struct ShieldStruct 
	{
		bool Regens = false;
	};

	struct HealthStruct {
		int HealthRegenRate = 1;
		int HealthMax = 100;
		int Health = 100;
	};
};