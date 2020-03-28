#include "Ammo.h"
#include "Player.h"
Ammo::Ammo(int _ammoAmount) 
{
	m_ammoAmount = _ammoAmount;
	
}
Ammo::~Ammo() {}

bool Ammo::Interaction(Thing* _thingInitatingInteraction)
{
	
	return false;
}

void Ammo::InteractionOnEnteringTile(Thing* _thingInitatingInteraction)
{
	if (m_allowInteraction)
	{
		Player* p = dynamic_cast<Player*>(_thingInitatingInteraction);
		if (p != nullptr)
		{
			if (p->AddAmmo(m_ammoAmount))
			{
				m_shouldRender = false;
				m_allowInteraction = false;
			}
		}
	}
	
}


