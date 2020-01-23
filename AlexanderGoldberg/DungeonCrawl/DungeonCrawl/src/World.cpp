#include "World.h"
#include "Tile.h"


World::World(int _width, int _height) 
{
	m_horizontalTileCount = _width;
	m_verticalTileCount = _height;
}
World::~World() {}
void World::AddTile(Tile* _tile)
{
	m_tiles.push_back(_tile);	
}

int World::GetTileCount() {
	return m_tiles.size();
}

bool World::IsViableDirectionToMoveIn(int _currentTileIndex, TileDirection _direction)
{
	int targIndex = 0;
	switch (_direction)
	{
	case TileDirection::UP:
		targIndex = _currentTileIndex - m_horizontalTileCount;
		//not in top row, is passable
		if (_currentTileIndex < m_horizontalTileCount || (m_tiles.size() > 0 && !m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::DOWN:
		targIndex = _currentTileIndex + m_horizontalTileCount;
		//not in bottom row, is passable
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount || (m_tiles.size() > 0 && !m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::LEFT:
		targIndex = _currentTileIndex - 1;
		//does not carry over to previous row, is in bounds, is passible
		if (targIndex < 0 || 
			targIndex % m_horizontalTileCount == m_horizontalTileCount -1 ||
			(m_tiles.size() > 0 && !m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::RIGHT:
		targIndex = _currentTileIndex + 1;
		//does not carry over to next row, is in bounds, is passible
		if (targIndex % m_horizontalTileCount == 0 ||
			targIndex > m_tiles.size() -1 ||
			(m_tiles.size() > 0 &&
			!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::UPLEFT:
		targIndex = _currentTileIndex - m_horizontalTileCount - 1;
		//not in top row, in bounds, does not carry over to end of above row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			_currentTileIndex -1 > -1 ||
			targIndex % m_horizontalTileCount == (m_horizontalTileCount - 1) ||
			(m_tiles.size() > 0 &&
			!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::UPRIGHT:
		targIndex = _currentTileIndex - m_horizontalTileCount + 1;
		//not in top row, does not carry over to current row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == 0 ||
			(m_tiles.size() > 0 &&
			!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::DOWNLEFT:
		//not in bottom row, does not carry over to end of current row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount - 1;
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == m_horizontalTileCount  -1 ||
			(m_tiles.size() > 0 &&
			!m_tiles[targIndex]->IsPassable()))
			return false;
		break;
	case TileDirection::DOWNRIGHT:
		//not in bottom row, does not carry over to beginning of next row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount + 1;
		if(_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount  ||
			targIndex % m_horizontalTileCount == 0 ||
			(m_tiles.size() > 0 &&
			!m_tiles[(targIndex)]->IsPassable()))
		{
			return false;
		}
	
		break;
	default:
		printf("Collision check resulted in direction hitting default case.\n");
		break;
	}
	return true;
}