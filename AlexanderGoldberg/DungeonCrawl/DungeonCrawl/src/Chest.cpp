#include "Chest.h"
#include "World.h"
#include "Tile.h"
#include "Ammo.h"
#include "HealthPickUp.h"
Chest::Chest()
{
}
Chest::Chest(World* _world, SDL_Renderer* _renderer)
{
	m_world = _world;
	m_rendererRef = _renderer;
}
Chest::~Chest()
{
}

bool Chest::Interaction(Thing* _thingInitatingInteraction)
{
	if (m_world != nullptr)
	{
		Tile* locationIndex = m_world->GetTileAtIndex(m_world->GetRandomTileInRoom(m_world->GetIndexOfRoomTileIsIn(GetLocation()->GetPositionInVector())));
		std::vector<Tile*> neighbors = m_world->GetNeighbors(locationIndex->GetPositionInVector());
		Tile* t = neighbors[rand() % neighbors.size()];
		if (rand() % 2 == 0)
		{
			Ammo* a = new Ammo((rand() %10) + 5);
			a->Init("img/pics/clip.png", "AmmoClip", m_rendererRef);
			t->AddItem(a);
		}
		else
		{
			HealthPickUp* a = new HealthPickUp((rand() % 10) + 5);
			a->Init("img/pics/health_small.png", "Health Pick up", m_rendererRef);
			t->AddItem(a);
		}
		m_allowInteraction = false;
		m_shouldRender = false;
	}
	return false;
}
