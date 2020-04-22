#pragma once
#include "Thing.h"
#include "Timer.h"
#include <vector>
#include "Enums.h"
using namespace Enums;
class Damagable :
	public Thing
{
protected:
	int m_health = 10;
	int m_healthMax = 10;
	Timer m_regenTimer = Timer(0.5f);
	int m_regenRate = 0;
	int m_regenAmount = 0;
	std::vector<DamageType> m_vulnarabilites;
	std::vector<DamageType> m_resitances;
	bool vectorContainsDamageType(const std::vector<DamageType>& _vectorToCheck, DamageType _damageTypeToCheckFor);
	virtual void die();
public:
	void SetHealthMax(int _healthMax, bool _effectcurrentHealth = true);
	void SetHealth(int _health, bool _effectmaxHealth = false);
	void SetRegenRate(int _regenRate);
	void SetRegenAmount(int _regenAmount);
	Damagable();
	~Damagable();
	virtual bool TakeDamage(int _amount, DamageType _type); //returns true if damage reduces health to 0 or less
	virtual bool Heal(int _amount); // returns true if already at max health
	bool IsDead();
	float GetHealth();
	float GetHealthAsPercent();
	float GetPercentOfTotalHealth(int _percent0To100);
	float GetOnePercentOfCurrentHealth(int _percent0To100);
	float GetPercentOfTotalHealth(float _percent0To1);
	float GetOnePercentOfCurrentHealth(float _percent0To1);
	
	void Update(float _dt) override;
	void PrintDamageableInfo();
};

