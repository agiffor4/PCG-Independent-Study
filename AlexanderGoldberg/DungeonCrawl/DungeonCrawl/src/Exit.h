#pragma once
#include "Interactable.h"
class World;
class Exit :
	public Interactable
{
protected:
	World* m_world;
public:
	Exit(World* _world);
	~Exit();
	bool Interaction() override;
};

