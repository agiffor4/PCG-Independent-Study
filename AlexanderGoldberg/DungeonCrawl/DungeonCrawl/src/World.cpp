#include "World.h"
#include "Tile.h"
#include "Scene.h"
#include "InputManager.h"
#include "Player.h"
#include "BSP.h"
#include "AStarSearch.h"
#include"Player.h"
#include "Exit.h"

World::World(int _hTileCount, int _vTileCount, Scene* _scene)
{
	m_horizontalTileCount = _hTileCount;
	m_verticalTileCount = _vTileCount;
	m_scene = _scene;
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
	if (m_tiles.size() < 1)
		return false;
	int targIndex = 0;
	switch (_direction)
	{
	case TileDirection::UP:
		targIndex = _currentTileIndex - m_horizontalTileCount;
		//not in top row, is passable
		if (_currentTileIndex < m_horizontalTileCount || (!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::DOWN:
		targIndex = _currentTileIndex + m_horizontalTileCount;
		//not in bottom row, is passable
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount || (!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::LEFT:
		targIndex = _currentTileIndex - 1;
		//does not carry over to previous row, is in bounds, is passible
		if (targIndex < 0 || 
			targIndex % m_horizontalTileCount == m_horizontalTileCount -1 ||
			(!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::RIGHT:
		targIndex = _currentTileIndex + 1;
		//does not carry over to next row, is in bounds, is passible
		if (targIndex % m_horizontalTileCount == 0 ||
			targIndex > m_tiles.size() -1 ||
			(!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::UPLEFT:
		targIndex = _currentTileIndex - m_horizontalTileCount - 1;
		//not in top row, in bounds, does not carry over to end of above row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			targIndex < 0 ||
			targIndex % m_horizontalTileCount == (m_horizontalTileCount - 1) ||
			(!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::UPRIGHT:
		targIndex = _currentTileIndex - m_horizontalTileCount + 1;
		//not in top row, does not carry over to current row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == 0 ||
			(!m_tiles[targIndex]->IsPassable()))
		{
			return false;
		}
		break;
	case TileDirection::DOWNLEFT:
		//not in bottom row, does not carry over to end of current row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount - 1;
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == m_horizontalTileCount - 1 ||
			(!m_tiles[targIndex]->IsPassable()))
			{
				return false;
			}
		break;
	case TileDirection::DOWNRIGHT:
		//not in bottom row, does not carry over to beginning of next row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount + 1;
		if(_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount  ||
			targIndex % m_horizontalTileCount == 0 ||
			(!m_tiles[(targIndex)]->IsPassable()))
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
	if (_currentTileIndex < 0 || _currentTileIndex > m_tiles.size())
	{
		printf("_currentTileIndex is out of bounds\n");
		return nullptr;
	}
	int targIndex = 0;
	switch (_direction)
	{
	case TileDirection::UP:
		targIndex = _currentTileIndex - m_horizontalTileCount;
		//not in top row, is passable
		if (_currentTileIndex < m_horizontalTileCount)
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::DOWN:
		targIndex = _currentTileIndex + m_horizontalTileCount;
		//not in bottom row, is passable
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount)
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::LEFT:
		targIndex = _currentTileIndex - 1;
		//does not carry over to previous row, is in bounds, is passible
		if (targIndex < 0 ||
			targIndex % m_horizontalTileCount == m_horizontalTileCount - 1)
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::RIGHT:
		targIndex = _currentTileIndex + 1;
		//does not carry over to next row, is in bounds, is passible
		if (targIndex % m_horizontalTileCount == 0 ||
			targIndex > m_tiles.size() - 1)
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::UPLEFT:
		targIndex = _currentTileIndex - m_horizontalTileCount - 1;
		//not in top row, in bounds, does not carry over to end of above row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			targIndex < 0 ||
			targIndex % m_horizontalTileCount == (m_horizontalTileCount - 1))
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::UPRIGHT:
		targIndex = _currentTileIndex - m_horizontalTileCount + 1;
		//not in top row, does not carry over to current row, is passable
		if (_currentTileIndex < m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == 0)
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::DOWNLEFT:
		//not in bottom row, does not carry over to end of current row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount - 1;
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == m_horizontalTileCount - 1)
		{
			return nullptr;
		}
		return m_tiles[targIndex];
	case TileDirection::DOWNRIGHT:
		//not in bottom row, does not carry over to beginning of next row, is passable
		targIndex = _currentTileIndex + m_horizontalTileCount + 1;
		if (_currentTileIndex > (m_tiles.size() - 1) - m_horizontalTileCount ||
			targIndex % m_horizontalTileCount == 0)
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


Vector2 World::GetTileSize(){
	return m_tiles[0]->GetCurrentSize();
}

void World::GenerateTiles(int _screenWidth, int _screenHeight) {
	Vector2 targetSize = Vector2(_screenWidth / m_horizontalTileCount, _screenHeight / m_verticalTileCount);
	
	for (size_t i = 0; i < m_verticalTileCount; i++)
	{
		for (size_t j = 0; j < m_horizontalTileCount; j++)
		{

			std::string name = "Tile (" + std::to_string(j) + ", " + std::to_string(i) + ")";
			Tile* t = new Tile();
			t->Init("img/blank_tile.bmp", name, GetTileCount(), j, i, Vector2(j * targetSize.X, i * targetSize.Y), m_scene->GetRenderer());
			t->SetPassable(false);
			t->SetSize(targetSize);
			AddTile(t);
			InputManager::GetInputManager()->SubscribeToInput(t, InputManager::KeyPressType::MOUSEUP);
			m_scene->AddRenderable(t);
		}
	}
}

void World::clearPreviousLevel() {
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		
		m_tiles[i]->ClearTileContents();
	}
}

void World::GenerateLevel()
{
	clearPreviousLevel();
	BSP bsp = BSP(m_horizontalTileCount, m_verticalTileCount);
	//when width = height = x round(log(x / 3) / log(2)) = number of times to split	
	//bsp.BeginSplit(round(std::log(m_horizontalTileCount / 3) / std::log(2))); //seems to produce a good ratio of rooms as long as the width = height
	bsp.BeginSplit(4);
	
	AStarSearch AStar = AStarSearch();	
	AStar.Initialize(GetMapDimentions(), GetTileCount(), false);
	AStar.SetWallDigCost(200);		
	BSP::TunnelingType tuntype = (BSP::TunnelingType)(rand() % 6);
	//BSP::TunnelingType tuntype = BSP::TunnelingType::Sequential;
	printf("Using tunneling algorithm %s.\n", bsp.GetEnumName(tuntype).c_str());
	bsp.SetTunnelingType(tuntype);
	std::vector<std::vector<int>> rooms = bsp.GetRoomTileIndexes();
	AddRooms(rooms);
	AStar.CastTilesToAStarNodes((*this));
	std::vector<int> paths = bsp.GeneratePaths(AStar);	
	AddPaths(paths);
	//bsp.GenerateRoomsAndPaths(AStar, rooms, paths);
	//AddRoomsAndPaths(rooms, paths);
	if (!m_playerCreated)
		m_player = CreatePlayer();
	int roomPlayerSpawnin;
	int playerStart = GetPlayerStartLocation(rooms, &roomPlayerSpawnin);
	Tile* t = GetTileAtIndex(playerStart);
	m_player->SetLocation(t);
	t->SetContents(m_player);
	
	//EXIT generation
	/*t = GetTileAtIndex(bsp.GenerateExitLocation(playerStart, roomPlayerSpawnin, (*this)));
	Exit* e = new Exit(this);
	e->Init("img/Exit.bmp", "Exit", m_scene->GetRenderer());
	e->SetSize(GetTileSize().X, GetTileSize().Y);
	e->SetLocation(t);
	t->AddItem(e);*/
	

}


void World::AddRoomsAndPaths(std::vector<std::vector<int>>& const _rooms, std::vector<int>& const _paths) {
	AddRooms(_rooms);
	AddPaths(_paths);
}

void World::AddRooms(std::vector<std::vector<int>>& const _rooms) {
	for (size_t i = 0; i < _rooms.size(); i++)
	{
		for (size_t j = 0; j < _rooms[i].size(); j++)
		{
			GetTileAtIndex(_rooms[i][j])->SetPassable(true);
			GetTileAtIndex(_rooms[i][j])->changeImage("img/blank_tile" + std::to_string(i) + ".bmp");
		}
	}
}

void World::AddPaths(std::vector<int>& const _paths) {

	for (size_t j = 0; j < _paths.size(); j++)
	{
		if (!GetTileAtIndex(_paths[j])->IsPassable())
			GetTileAtIndex(_paths[j])->SetCorridor(true);
		GetTileAtIndex(_paths[j])->SetPassable(true);
		
	}
}

int World::GetPlayerStartLocation(const std::vector<std::vector<int>>& _rooms, int* roomSpawnedIn) {
	int roomToSpawnIn = rand() % _rooms.size();
	int tileInRoom = rand() % _rooms[roomToSpawnIn].size();
	(*roomSpawnedIn) = roomToSpawnIn;
	return _rooms[roomToSpawnIn][tileInRoom];
}

Player* World::CreatePlayer()
{
		m_playerCreated = true;		
		Player* p = new Player();		
		p->Initalize((*this), "img/Player.bmp", "Player", m_scene->GetRenderer());
		return p;
	
}

void World::InvokeKeyUp(SDL_Keycode _key)
{
	switch (_key)
	{
	case SDLK_g:
		GenerateLevel();
		break;
	default:
		break;
	}
}
	


