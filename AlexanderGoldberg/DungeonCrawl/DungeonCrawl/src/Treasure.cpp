#include "Treasure.h"
#include "Tile.h"
Treasure::Treasure() {
	m_blocksPassage = false;
}
Treasure::~Treasure() {}

bool Treasure::Interaction(Thing* _thingInitatingInteraction)
{
	m_flagForDelation = true;
	return false;
}
