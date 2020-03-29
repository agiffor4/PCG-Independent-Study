#include "Chest.h"
#include "World.h"
#include "Tile.h"
#include "Ammo.h"
#include "HealthPickUp.h"
#include "Player.h"
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
		std::vector<Tile*> neighbors = m_world->GetNeighbors(GetLocation()->GetPositionInVector());
		Tile* t = nullptr;
		do
		{
			t = neighbors[rand() % neighbors.size()];

		} while (t == nullptr || !t->IsPassable());
		Player* p = dynamic_cast<Player*>(_thingInitatingInteraction);
		if (p != nullptr)
		{		
			if (p->GetHealthAsPercent() < p->GetAmmoAsPercent())
			{
				Ammo* a = new Ammo((rand() % 10) + 5);
				a->Init("img/pics/clip.png", "AmmoClip", m_rendererRef);
				t->AddItem(a);
			}
			else if (p->GetHealthAsPercent() >  p->GetAmmoAsPercent())
			{
				HealthPickUp* a = new HealthPickUp((rand() % 10) + 5);
				a->Init("img/pics/health_small.png", "Health Pick up", m_rendererRef);
				t->AddItem(a);
			}
			else
			{
				if (rand() % 2 == 0)
				{
					Ammo* a = new Ammo((rand() % 10) + 5);
					a->Init("img/pics/clip.png", "AmmoClip", m_rendererRef);
					t->AddItem(a);
				}
				else 
				{
					HealthPickUp* a = new HealthPickUp((rand() % 10) + 5);
					a->Init("img/pics/health_small.png", "Health Pick up", m_rendererRef);
					t->AddItem(a);
				}
			}

		}
		m_allowInteraction = false;
		m_shouldRender = false;
	}
	return false;
}
