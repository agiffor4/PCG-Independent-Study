#include "Key.h"
#include "DoorA.h"
void Key::SetDoor(DoorA* _door)
{
	m_door.push_back(_door);
}
Key::Key() {}
Key::~Key() {}

bool Key::Interaction(Thing* _thingInitatingInteraction)
{
	
		if (m_door.size() > 0)
		{
			for (size_t i = 0; i < m_door.size(); i++)
			{
				m_door[i]->SetBlocksPassage(false);
			}			
		}
		m_shouldRender = false;
		m_allowInteraction = false;
		
	return false;
}
