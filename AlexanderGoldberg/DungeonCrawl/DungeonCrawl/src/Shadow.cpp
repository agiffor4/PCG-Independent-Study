#include "Shadow.h"
#include "Tile.h"
#include "Camera.h"
Shadow::Shadow()
{
	//m_allowInteraction = false;
}

Shadow::~Shadow()
{
}
/*
bool Shadow::Interaction(Thing* _thingInitatingInteraction)
{
	return false;
}*/
bool Shadow::inBounds(int _x, int _y) {
	if (_x > GetDestination().x - Camera::Offset().X && _x < GetDestination().x + GetDestination().w - Camera::Offset().X &&
		_y > GetDestination().y - Camera::Offset().Y && _y < GetDestination().y + GetDestination().h - Camera::Offset().Y)
	{
		return true;
	}
	return false;
}

