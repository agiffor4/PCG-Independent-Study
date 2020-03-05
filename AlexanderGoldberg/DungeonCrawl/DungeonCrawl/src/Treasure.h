#pragma once
#include "Interactable.h"
class Treasure :
	public Interactable
{
public:
	Treasure();
	~Treasure();
	bool Interaction() override;
};

