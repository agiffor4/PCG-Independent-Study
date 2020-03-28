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

bool Thing::IsSolid()
{
	return m_solid;
}

bool Thing::ShouldCheckCollision()
{
	return m_checkCollision;
}

void Thing::CheckCollision(Thing* _other)
{	
	if (Vector2::GetDistanceLessThan(_other->getCenterOfTexture(), getCenterOfTexture(), (_other->GetDestination().w > GetDestination().w ? _other->GetDestination().w : GetDestination().w) * 0.5f))
	{
		OnCollision(_other);
	}
}

void Thing::OnCollision(Thing* _other)
{

}
