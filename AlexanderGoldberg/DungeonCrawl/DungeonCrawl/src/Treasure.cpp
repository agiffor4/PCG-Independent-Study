#include "Treasure.h"
#include "Tile.h"
Treasure::Treasure() {
	m_blocksPassage = false;
}
Treasure::~Treasure() {}

bool Treasure::Interaction()
{
	m_flagForDelation = true;
	return false;
}
