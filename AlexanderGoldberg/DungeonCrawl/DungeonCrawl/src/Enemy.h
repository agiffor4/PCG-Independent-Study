#pragma once
#include "StatusEffectable.h"
#include "EnemyMovement.h"
#include "EnemyCombat.h"
#include "EnemyDataStructs.h"
class Enemy :
	public StatusEffectable
{
public:
	enum class EnemyProperties 
	{
		combatRanged = 1, //ranged combat exclusive with melee combat
		combatMelee = 2, //melee combat exclusive with ranged combat
		combatSummon = 4, //summons other enemies, not exclusive with other combat properties
		movemetMoves = 8, //if true allows movement, if false stationary
		defenseShieldTimed,
		defenseShieldBreakable,
		defenseLeaveBarricades,
		healthRegen,



	};
protected:
	EnemyDataStructs::ShieldStruct m_shieldData = EnemyDataStructs::ShieldStruct();
	EnemyMovement m_movement = EnemyMovement();
	EnemyCombat m_combat = EnemyCombat();
	Uint32 m_EnemyProfile = 0;


	//UTILITY
	int getRandomInRange(int _min, int _max);
	bool propertyInProfile(EnemyProperties _property);
	void addPropertyToProfile(EnemyProperties _property);
	void removePropertyFromProfile(EnemyProperties _property);
	Vector2 rotateDirectionByDegrees(Vector2 _direction, float _degrees);

public:
	Enemy();
	~Enemy();
};

