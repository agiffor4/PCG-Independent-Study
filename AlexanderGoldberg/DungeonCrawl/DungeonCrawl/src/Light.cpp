#include "Light.h"
#include "World.h"
#include "Tile.h"
#include "Player.h"
#include <set>
Light::Light()
{
}
Light::~Light() {}
void Light::SetLightRadius(int _lightRadiusInTiles)
{
	m_lightRadiusInTiles = _lightRadiusInTiles;
}
void Light::SetWorld(World* _world) {
	m_world = _world;
}
void Light::Illuminate(bool _lightUp) {
	if (m_world != nullptr)
	{
		Tile* epicenter = nullptr;
		if (m_holder == nullptr)
		{
			if (GetLocation() != nullptr)
			{
				epicenter = GetLocation();
			}
		}
		else
		{
			epicenter = m_holder->GetLocation();
		}


		if (epicenter != nullptr)
		{	
			std::set<Tile*> inLight = GetEffectedTiles(epicenter);
			for (auto i = inLight.begin();  i != inLight.end(); i++)
			{
				(*i)->SetIlluminated(_lightUp);
			}
		}
	}
}

void Light::SetLocation(Tile* _newLocation)
{
	
	Illuminate(false);
	Thing::SetLocation(_newLocation);
	Illuminate(true);
}

std::set<Tile*> Light::GetEffectedTiles(Tile* _epicenter)
{
	std::set<Tile*> toIlluminate;
	int center = _epicenter->GetPositionInVector();
	
	std::vector<Tile*> neigbors = m_world->GetNeighbors(center);
	for (size_t i = 0; i < neigbors.size(); i++)
	{
		toIlluminate.emplace(neigbors[i]);
	}
	std::vector<Tile*> tempNeigbors;
	for (size_t k = 0; k < m_lightRadiusInTiles; k++)
	{

		for (size_t i = 0; i < tempNeigbors.size(); i++)
			toIlluminate.emplace(tempNeigbors[i]);
		tempNeigbors.clear();
		for (auto itt = toIlluminate.begin(); itt != toIlluminate.end(); itt++)
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

	
	return toIlluminate;
}

bool Light::Interaction()
{
	return false;
}



