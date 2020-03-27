#pragma once
#include "Interactable.h"
class HealthPickUp :
	public Interactable
{
private:
	int m_regenAmount = 5;
public:
	HealthPickUp(int _regenAmount);
	~HealthPickUp();
	bool Interaction(Thing* _thingInitatingInteraction) override;
};
