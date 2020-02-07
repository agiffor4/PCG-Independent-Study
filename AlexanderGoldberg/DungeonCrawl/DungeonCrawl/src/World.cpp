#include "World.h"
#include "Tile.h"
#include "Scene.h"
#include "InputManager.h"

World::World(int _hTileCount, int _vTileCount)
{
	m_horizontalTileCount = _hTileCount;
	m_verticalTileCount = _vTileCount;
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

Tile* World::GetAdjacentTile(int _currentTileIndex, TileDirection _direction)
{
	int targIndex = 0;
	switch (_direction)
	{
	case TileDirection::UP:
		targIndex = _currentTileIndex - m_horizontalTileCount;
		//not in top row, is passable
		if (_currentTileIndex < m_horizontalTileCount || (m_tiles.size() > 0))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::DOWN:
		targIndex = _currentTileIndex + m_horizontalTileCount;
		//not in bottom row, is passable
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount || (m_tiles.size() > 0))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::LEFT:
		targIndex = _currentTileIndex - 1;
		//does not carry over to previous row, is in bounds, is passible
		if (targIndex < 0 ||
			targIndex % m_horizontalTileCount == m_horizontalTileCount - 1 ||
			(m_tiles.size() > 0 && !m_tiles[targIndex]->IsPassable()))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::RIGHT:
		targIndex = _currentTileIndex + 1;
		//does not carry over to next row, is in bounds, is passible
		if (targIndex % m_horizontalTileCount == 0 ||
			targIndex > m_tiles.size() - 1 ||
			(m_tiles.size() > 0 &&
				!m_tiles[targIndex]->IsPassable()))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::UPLEFT:
		targIndex = _currentTileIndex - m_horizontalTileCount - 1;
		//not in top row, in bounds, does not carry over to end of above row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			_currentTileIndex - 1 > -1 ||
			targIndex % m_horizontalTileCount == (m_horizontalTileCount - 1) ||
			(m_tiles.size() > 0 &&
				!m_tiles[targIndex]->IsPassable()))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::UPRIGHT:
		targIndex = _currentTileIndex - m_horizontalTileCount + 1;
		//not in top row, does not carry over to current row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == 0 ||
			(m_tiles.size() > 0 &&
				!m_tiles[targIndex]->IsPassable()))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::DOWNLEFT:
		//not in bottom row, does not carry over to end of current row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount - 1;
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == m_horizontalTileCount - 1 ||
			(m_tiles.size() > 0 &&
				!m_tiles[targIndex]->IsPassable()))
			return nullptr;
		return m_tiles[targIndex];
	case TileDirection::DOWNRIGHT:
		//not in bottom row, does not carry over to beginning of next row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount + 1;
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == 0 ||
			(m_tiles.size() > 0 &&
				!m_tiles[(targIndex)]->IsPassable()))
		{
			return nullptr;
		}

		return m_tiles[targIndex];
	default:
		printf("Collision check resulted in direction hitting default case.\n");
		return nullptr;
	}
	
}

Tile* World::GetTileAtIndex(int _index) 
{
	return _index > -1 && _index < m_tiles.size() ? m_tiles[_index] : nullptr;
}

Tile* World::GetTileAtPosition(int _x, int _y)
{
	int targetTOReturn = (m_horizontalTileCount * _y) + _x;
	if (targetTOReturn < m_tiles.size())
	{
		if (_y <= m_verticalTileCount)
		{
			if (_x <= m_horizontalTileCount)
			{
				return m_tiles[targetTOReturn];
			}
			else
			{
				return m_tiles[m_horizontalTileCount * (_y+1)];
			}
		}
		else
		{
			if (m_tiles.size() - m_horizontalTileCount + _x < m_tiles.size())
			{
				return m_tiles[m_tiles.size() - m_horizontalTileCount + _x];
			}
		}
		
	}
	return m_tiles[m_tiles.size() - 1];
}

Tile* World::GetTile(Tile& _tile)
{
	return GetTileAtIndex(_tile.GetPositionInVector());
}

Vector2 World::GetMapDimentions() 
{
	return Vector2(m_horizontalTileCount, m_verticalTileCount);
}

std::vector<Tile*> World::GetNeighbors(Tile* _tileToFindNeighborsFor) 
{
	return GetNeighbors(_tileToFindNeighborsFor->GetPositionInVector());
}

std::vector<Tile*> World::GetNeighbors(int _tileToFindNeighborsFor)
{
	std::vector<Tile*> neighbors;
	for (size_t i = 0; i < 8; i++)
	{
		Tile* currentCheck = GetAdjacentTile(_tileToFindNeighborsFor, (TileDirection)i);
		if (currentCheck != nullptr)
		{
			neighbors.push_back(currentCheck);
		}
	}
	return neighbors;
}

std::vector<Tile*> World::GetTiles()
{
	return m_tiles;;
}


void World::GenerateTiles(Scene* _scene, int _screenWidth, int _screenHeight) {
	Vector2 targetSize = Vector2(_screenWidth / m_horizontalTileCount, _screenHeight / m_verticalTileCount);
	
	for (size_t i = 0; i < m_verticalTileCount; i++)
	{
		for (size_t j = 0; j < m_horizontalTileCount; j++)
		{

			std::string name = "Tile (" + std::to_string(j) + ", " + std::to_string(i) + ")";
			Tile* t = new Tile();
			t->Init("img/blank_tile.bmp", name, GetTileCount(), j, i, Vector2(j * targetSize.X, i * targetSize.Y), _scene->GetRenderer());
			t->SetPassable(false);
			t->SetSize(targetSize);
			AddTile(t);
			InputManager::GetInputManager()->SubscribeToInput(t, InputManager::KeyPressType::MOUSEUP);
			_scene->AddRenderable(t);
		}
	}
}