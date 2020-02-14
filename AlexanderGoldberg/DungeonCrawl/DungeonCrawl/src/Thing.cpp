#include "Thing.h"
#include "Tile.h"
Thing::Thing()
{
}

Thing::~Thing()
{
}

void Thing::SetLocation(Tile* _newLocation)
{
	m_location = _newLocation;
}

Tile* Thing::GetLocation()
{
	return m_location;
}

bool Thing::ShouldDelete() {
	return m_shouldDelete;
}