#pragma once
#include "StatusEffectable.h"
#include "Vector2.h"
class WeaponHolder :
	public StatusEffectable
{
protected:
	Vector2  m_aimDirection = Vector2();
	void setAimDirection(Vector2& _aimDirection);
	void setAimDirection(Vector2 _aimDirection);
public:
	Vector2 GetAimDirection();

};

