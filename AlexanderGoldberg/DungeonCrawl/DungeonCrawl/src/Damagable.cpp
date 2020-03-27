#include "Damagable.h"

void Damagable::die()
{
}

void Damagable::SetHealthMax(int _healthMax, bool _effectcurrentHealt)
{
	m_healthMax = _healthMax;
	if (_effectcurrentHealt)
	{
		m_health = m_healthMax;
	}
}

void Damagable::SetHealth(int _health, bool _effectMaxHealth)
{
	m_health = _health;
	if (m_health > m_healthMax)
	{
		if (_effectMaxHealth)
			m_healthMax = m_health;
		else
			m_health = m_healthMax;
	}
	
}


void Damagable::SetRegenRate(int _regenRate)
{
	m_regenRate = _regenRate;
}

void Damagable::SetRegenAmount(int _regenAmount)
{
	m_regenAmount = _regenAmount;
}

Damagable::Damagable()
{
}

Damagable::~Damagable()
{
}

bool Damagable::TakeDamage(int _amount)
{
	m_health -= _amount;
	if (m_health < 0)
		m_health = 0;
	if (m_health == 0)
	{
		die();
		return true;
	}
	return false;
}

bool Damagable::Heal(int _amount)
{
	if (m_health == m_healthMax)
	{
		return false;
	}
	m_health += _amount;
	if (m_health > m_healthMax)
		m_health = m_healthMax;
	return true;
}

bool Damagable::IsDead()
{
	return m_health == 0;
}

float Damagable::GetHealthAsPercent()
{
	return (float)m_health / (float)m_healthMax;
}

float Damagable::GetPercentOfTotalHealth(int _percent0To100)
{
	if (_percent0To100 < 0)
		_percent0To100 = 0;
	if (_percent0To100 > 100)
		_percent0To100 = 100;
	return (_percent0To100/100.0f) * m_healthMax;
}

float Damagable::GetOnePercentOfCurrentHealth(int _percent0To100)
{
	if (_percent0To100 < 0)
		_percent0To100 = 0;
	if (_percent0To100 > 100)
		_percent0To100 = 100;
	return (_percent0To100 / 100.0f) * m_health;
}

float Damagable::GetPercentOfTotalHealth(float _percent0To1)
{
	if (_percent0To1 < 0)
		_percent0To1 = 0;
	if (_percent0To1 > 1)
		_percent0To1 = 1;
	return _percent0To1 * m_healthMax;
}

float Damagable::GetOnePercentOfCurrentHealth(float _percent0To1)
{
	if (_percent0To1 < 0)
		_percent0To1 = 0;
	if (_percent0To1 > 1)
		_percent0To1 = 1;
	return _percent0To1 * m_health;
}

void Damagable::Update(float _dt)
{
	if (m_regenRate > 0 && m_health < m_healthMax)
	{
		if (m_regenTimer.CountDown(_dt))
		{
			Heal(m_regenAmount);
		}
	}
}
