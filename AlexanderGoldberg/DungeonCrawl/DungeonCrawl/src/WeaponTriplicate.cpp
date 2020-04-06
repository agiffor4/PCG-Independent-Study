#include "WeaponTriplicate.h"
#include "Scene.h"
WeaponTriplicate::WeaponTriplicate()
{
	
}

WeaponTriplicate::~WeaponTriplicate()
{
}

void WeaponTriplicate::Fire(Vector2 _direction)
{
	
	if (!m_triggerData.fireTimer.GetShouldCountDown())
	{
		double theta = 15.0f * deg2Rad;
			double cs = cos(theta);
			double sn = sin(theta);
			fire(_direction, true);
			float x = _direction.X * cs - _direction.Y * sn;
			float y = _direction.X * sn + _direction.Y * cs;
			_direction.X = x;
			_direction.Y = y;
			fire(_direction, false, true);

			theta = 330.0f * deg2Rad;
		cs = cos(theta);
		sn = sin(theta);
		x = _direction.X * cs - _direction.Y * sn;
		y = _direction.X * sn + _direction.Y * cs;
		_direction.X = x;
		_direction.Y = y;
		fire(_direction, false, true);
	}
	
}
