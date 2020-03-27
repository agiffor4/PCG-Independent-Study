#pragma once
#include "Thing.h"
class Shadow : public Thing
	//public Interactable
{
public:
	Shadow();
	~Shadow();
	//bool Interaction(Thing* _thingInitatingInteraction) override; //returns false if it should not be added to the player inventory ineteraction
	bool inBounds(int _x, int _y);
};

