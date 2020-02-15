#pragma once
#include "Thing.h"
class Interactable :
	public Thing
{
public:
	virtual bool Interaction() = 0;
};

