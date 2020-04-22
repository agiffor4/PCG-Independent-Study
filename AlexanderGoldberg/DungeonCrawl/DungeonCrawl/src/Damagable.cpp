#include "Damagable.h"

bool Damagable::vectorContainsDamageType(const std::vector<DamageType>& _vectorToCheck, DamageType _damageTypeToCheckFor)
{
	for (size_t i = 0; i < _vectorToCheck.size(); i++)
	{
		if (_vectorToCheck[i] == _damageTypeToCheckFor)
		{
			return true;
		}
	}
	return false;
}

void Damagable::die()
{
		
	printf("%s died\n", m_name.c_str());
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

bool Damagable::TakeDamage(int _amount, DamageType _type)
{
	if (vectorContainsDamageType(m_vulnarabilites, _type))
		_amount *= 2;
	if (vectorContainsDamageType(m_resitances, _type))
		_amount *= 0.5f;
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

float Damagable::GetHealth()
{
	return m_health;
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

void Damagable::PrintDamageableInfo()
{

	printf("Damageable: health %d/%d, regen rate %d, regen amount %d\n", m_health , m_healthMax, m_regenRate, m_regenAmount);
	printf("Vulnerable to: ");
	for (size_t i = 0; i < m_vulnarabilites.size(); i++)
	{
		printf(PrintDTName(m_vulnarabilites[i]).c_str());
		if (i < m_vulnarabilites.size() - 1)
			printf(", ");
		else
			printf("\n");
	}

	printf("Resistant to: ");
	for (size_t i = 0; i < m_resitances.size(); i++)
	{
		printf(PrintDTName(m_resitances[i]).c_str());
		if (i < m_resitances.size() - 1)
			printf(", ");
		else
			printf("\n");
	}

}
