#pragma once
#include "Interactable.h"
#include "IInputHandler.h"
class Shadow :
	public Interactable
{
public:
	Shadow();
	~Shadow();
	bool Interaction() override; //returns false if it should not be added to the player inventory ineteraction
	bool inBounds(int _x, int _y);
};

