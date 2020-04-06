#pragma once
#include "StatusEffectable.h"
#include "EnemyMovement.h"
#include "EnemyCombat.h"
class Enemy :
	public StatusEffectable
{
protected:
	EnemyMovement m_movement = EnemyMovement();
	EnemyCombat m_combat = EnemyCombat();
public:
	Enemy();
	~Enemy();
};

