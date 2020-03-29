#pragma once
#include "Interactable.h"
class World;
class Chest :
	public Interactable
{
private:
	Chest();
	World* m_world = nullptr;
public:
	Chest(World* _world, SDL_Renderer* _renderer);
	~Chest();
	bool Interaction(Thing* _thingInitatingInteraction) override;
};

