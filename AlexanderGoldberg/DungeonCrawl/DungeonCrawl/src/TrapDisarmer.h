#pragma once
#include "Interactable.h"
class TrapDisarmer :
	public Interactable
{
public:
	TrapDisarmer();
	~TrapDisarmer();
	bool Interaction(Thing* _thingInitatingInteraction) override; 
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
};

