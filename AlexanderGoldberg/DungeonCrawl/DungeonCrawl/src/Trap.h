#pragma once
#include "Interactable.h"
class Trap :
	public Interactable
{
protected:
	int m_damageInfliction = 5;
	int m_trapCharges = -1;
public:
	Trap();
	void SetTrapCharges(int _trapCharges);
	void SetTrapDamage(int _damageInfliction);
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
	
};

