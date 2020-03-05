#include "DoorA.h"
#include "Tile.h"
#include "Player.h"
DoorA::DoorA() 
{
	m_blocksPassage = true;
}
DoorA::~DoorA() {}

bool DoorA::Interaction()
{
	return false;
}

void DoorA::Update(float dt)
{
	if (m_location->GetContents() != nullptr && !m_blocksPassage)
	{
		if (dynamic_cast<Player*>(m_location->GetContents()) != nullptr)
		{
			m_shouldRender = false;
		}
	}
}
