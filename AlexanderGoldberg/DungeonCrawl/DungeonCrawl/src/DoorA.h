#pragma once
#include "Interactable.h"
class DoorA :
	public Interactable
{
public:
	DoorA();
	~DoorA();
	bool Interaction() override;
};

