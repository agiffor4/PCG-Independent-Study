#pragma once
#include "Weapon.h"
class WeaponTriplicate :
	public Weapon
{
public:
	WeaponTriplicate();
	~WeaponTriplicate();
	void Fire(Vector2 _direction) override;
};

