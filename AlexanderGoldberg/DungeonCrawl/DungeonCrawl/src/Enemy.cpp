#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}



int Enemy::getRandomInRange(int _min, int _max)
{
	if (_max < _min)
	{
		printf("_min out of bounds! returning -1.\n");
		return -1;
	}
	return (rand() % (_max + 1 - _min)) + _min;
}

bool Enemy::propertyInProfile(EnemyProperties _property)
{
	return (m_EnemyProfile & (Uint32)_property);
}

void Enemy::addPropertyToProfile(EnemyProperties _property)
{
	m_EnemyProfile |= (Uint32)_property;
}

void Enemy::removePropertyFromProfile(EnemyProperties _property)
{
	m_EnemyProfile &= ~(Uint32)_property;
}

Vector2 Enemy::rotateDirectionByDegrees(Vector2 _direction, float _degrees)
{
	while (_degrees < 0)
		_degrees += 360;
	while (_degrees > 360)
		_degrees -= 360;
	double theta = _degrees * deg2Rad;
	double cs = cos(theta);
	double sn = sin(theta);
	float x = _direction.X * cs - _direction.Y * sn;
	float y = _direction.X * sn + _direction.Y * cs;
	_direction.X = x;
	_direction.Y = y;
	return _direction;
}
