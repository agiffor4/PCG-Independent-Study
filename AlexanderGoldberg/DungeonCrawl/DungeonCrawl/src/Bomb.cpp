#include "Bomb.h"
#include "Tile.h"
#include "World.h"
#include "Damagable.h"
Bomb::Bomb(World* _world)
{
	m_world = _world;
}

void Bomb::Throw(Thing* _thrower, bool _damagesThrower)
{
	m_thrower = _thrower;
	m_damagesThrower = _damagesThrower;
}

bool Bomb::Interaction(Thing* _thingInitatingInteraction)
{
	return false;
}

void Bomb::InteractionOnEnteringTile(Thing* _thingInitatingInteraction)
{

}

void Bomb::Detonate()
{
	std::set<Tile*> effected = GetEffectedTiles(GetLocation());
	for (Tile* t : effected)
	{
		if (t->GetContents() != nullptr)
		{
			if (m_damagesThrower || (m_thrower != t->GetContents()))
			{
				Damagable* d = dynamic_cast<Damagable*>(t->GetContents());
				if (d != nullptr)
				{
					d->TakeDamage(m_damageInflicted);
				}
			}
		}
		for (size_t i = 0; i < t->GetItems().size(); i++)
		{
			Damagable* d = dynamic_cast<Damagable*>(t->GetItems()[i]);
			if (d != nullptr)
			{
				d->TakeDamage(m_damageInflicted);
			}
		}
	}
}

void Bomb::SetDamageOutput(int _damageInflicted)
{
	m_damageInflicted = _damageInflicted;
}

std::set<Tile*> Bomb::GetEffectedTiles(Tile* _epicenter)
{
	std::set<Tile*> toEffect;
	int center = _epicenter->GetPositionInVector();

	std::vector<Tile*> neigbors = m_world->GetNeighbors(center);
	for (size_t i = 0; i < neigbors.size(); i++)
	{
		toEffect.emplace(neigbors[i]);
	}
	std::vector<Tile*> tempNeigbors;
	for (size_t k = 0; k < m_radius; k++)
	{

		for (size_t i = 0; i < tempNeigbors.size(); i++)
			toEffect.emplace(tempNeigbors[i]);
		tempNeigbors.clear();
		for (auto itt = toEffect.begin(); itt != toEffect.end(); itt++)
		{
			std::vector<Tile*> templist = m_world->GetNeighbors((*itt)->GetPositionInVector());
			for (size_t i = 0; i < templist.size(); i++)
			{
				if (templist[i]->IsPassable())
				{
					tempNeigbors.push_back(templist[i]);
				}

			}

		}
	}
	toEffect.emplace(_epicenter);

	return toEffect;
}