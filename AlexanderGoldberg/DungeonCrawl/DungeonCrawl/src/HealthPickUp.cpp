#include "HealthPickUp.h"
#include "Damagable.h"

HealthPickUp::~HealthPickUp() {}
HealthPickUp::HealthPickUp(int _regenAmount)
{
	m_regenAmount = _regenAmount;
}

bool HealthPickUp::Interaction(Thing* _thingInitatingInteraction)
{
	Damagable* damage = dynamic_cast<Damagable*>(_thingInitatingInteraction);
	if (damage != nullptr)
	{
		if (damage->Heal(m_regenAmount))
		{
			m_shouldRender = false;
			m_allowInteraction = false;
		}
	}
	return false;
}