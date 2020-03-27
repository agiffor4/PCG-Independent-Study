#pragma once
#include "Interactable.h"
#include <set>
class World;
class Bomb :
	public Interactable
{
protected:
	int m_damageInflicted = 25;
	int m_radius = 1;
	World* m_world = nullptr;
public:
	
	Bomb(World* _world);
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
	void Detonate();
	void SetDamageOutput(int _damageInflicted);
	std::set<Tile*> GetEffectedTiles(Tile* _epicenter);
};


