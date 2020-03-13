#include "Light.h"
#include "World.h"
#include "Tile.h"
#include "Player.h"
#include "InputManager.h"
#include <set>
Light::Light() {
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::MOUSEUP);
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
	std::vector<Tile*> neighbors = m_world->GetNeighbors(_epicenter->GetPositionInVector(), true);
	std::vector<Tile*> toAddToIll;
	for (size_t j = 0; j < neighbors.size(); j++)
	{
		if (neighbors[j]->IsPassable(true))
		{
			toIlluminate.emplace(neighbors[j]);
		}
	}
	auto startPoint = toIlluminate.begin();
	if (m_lightRadiusInTiles > 1)
	{
		for (size_t i = 0; i < m_lightRadiusInTiles-1; i++)
		{
			neighbors.clear();
			toAddToIll.clear();
			for (auto k = startPoint; k != toIlluminate.end(); k++)
			{
				Tile* t = (*k);
				if (t->GetPositionInVector() == 556 ||
					t->GetPositionInVector() == 557 ||
					t->GetPositionInVector() == 558)
				{
					int foo = 0;
				}
				neighbors = m_world->GetNeighbors(t->GetPositionInVector());
				for (size_t l = 0; l < neighbors.size(); l++)
				{
					if (neighbors[l]->IsPassable(true))
						toAddToIll.push_back(neighbors[l]);
				}
					
			}
			//startPoint = toIlluminate.end();
			for (size_t j = 0; j < toAddToIll.size(); j++)
			{
				toIlluminate.emplace(toAddToIll[j]);
			}
		}
	}
	return toIlluminate;
}

bool Light::Interaction()
{
	return false;
}

void Light::InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y)
{
	switch (_mouse)
	{
	case IInputHandler::MouseButton::LEFT:
		Illuminate(true);
		break;
	case IInputHandler::MouseButton::RIGHT:
		Illuminate(false);
		break;
	case IInputHandler::MouseButton::MIDDLE:
		break;
	default:
		break;
	}
}
