#include "WeaponHolder.h"

void WeaponHolder::setAimDirection(Vector2& _aimDirection)
{
	m_aimDirection = _aimDirection;
}
void WeaponHolder::setAimDirection(Vector2 _aimDirection)
{
	m_aimDirection = _aimDirection;
}
Vector2 WeaponHolder::GetAimDirection()
{
	return m_aimDirection;
}
