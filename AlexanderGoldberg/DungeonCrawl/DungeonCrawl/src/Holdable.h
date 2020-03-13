#pragma once
#include "Interactable.h"
class Player;
class Holdable :
	public Interactable
{
protected:
	Player* m_holder = nullptr;
public:
	Holdable();
	~Holdable();
	bool Interaction() override;
};

