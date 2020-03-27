#pragma once
#include "Interactable.h"
class DoorA :
	public Interactable
{
public:
	DoorA();
	~DoorA();
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void Update(float dt) override;
};

