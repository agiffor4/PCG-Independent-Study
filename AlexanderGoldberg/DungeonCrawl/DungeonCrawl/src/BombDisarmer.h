#pragma once
#include "Interactable.h"
class BombDisarmer :
	public Interactable
{
public:
	BombDisarmer();
	~BombDisarmer();
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
};

