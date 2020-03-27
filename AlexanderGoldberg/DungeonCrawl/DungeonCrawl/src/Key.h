#pragma once
#include "Interactable.h"
#include <vector>
class DoorA;
class Key :
	public Interactable
{
protected:
	std::vector<DoorA*> m_door;;
public:
	void SetDoor(DoorA* _door);
	Key();
	~Key();
	bool Interaction(Thing* _thingInitatingInteraction) override;
};

