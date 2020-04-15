#include "Trap.h"
#include "Tile.h"
#include "Player.h"
Trap::Trap()
{
}
void Trap::SetTrapCharges(int _trapCharges)
{
	m_trapCharges = _trapCharges;
}
void Trap::SetTrapDamage(int _damageInfliction)
{
	m_damageInfliction = _damageInfliction;
}
bool Trap::Interaction(Thing* _thingInitatingInteraction) { return false; }
void Trap::ResetTrap()
{
	m_shouldRender = true;
}
void Trap::MoveTrapTo(Tile* _newTile)
{
	GetLocation()->RemoveItem(this, false);
	_newTile->AddItem(this);
	SetLocation(_newTile);
}
void Trap::InteractionOnEnteringTile(Thing* _thingInitatingInteraction)
{
	if (m_allowInteraction)
	{
		Player* p = dynamic_cast<Player*>(_thingInitatingInteraction);
		if (p != nullptr)
		{
			p->TakeDamage(m_damageInfliction, Enums::DamageType::Concussive);
		}
		if (m_trapCharges != -1)
		{
			m_trapCharges--;
			if (m_trapCharges == 0)
			{
				m_allowInteraction = false;
				m_shouldRender = false;
			}
		}
	}
}

