#pragma once
#include "Interactable.h"
class Ammo :
	public Interactable
{
protected:
	int m_ammoAmount = 5;
public: 
	Ammo(int _ammoAmount);
	~Ammo();
	bool Interaction(Thing* _thingInitatingInteraction) override; 
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
};

