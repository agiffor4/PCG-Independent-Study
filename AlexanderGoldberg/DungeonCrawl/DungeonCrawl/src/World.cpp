#include "World.h"
#include "Tile.h"
#include "Scene.h"
#include "InputManager.h"
#include "Player.h"
#include "BSP.h"
#include "AStarSearch.h"
#include"Player.h"
#include "Exit.h"
#include "DoorA.h"
#include "Key.h"

void World::setWindowTitle()
{
	if (m_window != nullptr)
	{
		BSP bsp = BSP(1, 1);
		std::string width = "width: " + std::to_string(m_horizontalTileCount) + " ";
		std::string height = "height: " + std::to_string(m_verticalTileCount) + " ";
		std::string tunnelT = "path: " + bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType) + " ";
		std::string seedResult = (m_resetSeed ? "T" : "F");
		std::string seed = " seed reset: " + seedResult + " ";
		std::string cycleResult = (m_cycleGenerationType ? "T" : "F");
		std::string cycle = " cycle:  " + cycleResult + " ";
		std::string ignoreResult = (m_ignoreExistingPaths ? "T" : "F");
		std::string ignore = " carve new: " + ignoreResult + " ";
		std::string incrementalResult = (m_digPathsOneAtATime ? "T" : "F");
		std::string incremental = " Incremental dig: " + incrementalResult + " ";

		std::string title = width + height + tunnelT + seed + cycle + ignore + incremental;
		SDL_SetWindowTitle(m_window, title.c_str());
	}
}



World::World(int _hTileCount, int _vTileCount, Scene* _scene)
{
	m_horizontalTileCount = _hTileCount;
	m_verticalTileCount = _vTileCount;
	m_scene = _scene;
}
World::~World() 
{
	if (m_bsp != nullptr)
		delete(m_bsp);
	if (m_AStar != nullptr)
		delete(m_AStar);
}
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

void World::SetWindowRef(SDL_Window* _window)
{
	m_window = _window;
}

std::vector<Tile*> World::GetNeighbors(Tile* _tileToFindNeighborsFor, bool _getDiagonals)
{
	return GetNeighbors(_tileToFindNeighborsFor->GetPositionInVector());
}

std::vector<Tile*> World::GetNeighbors(int _tileToFindNeighborsFor, bool _getDiagonals)
{
	std::vector<Tile*> neighbors;
	for (size_t i = 0; i < (_getDiagonals ? 8 : 4); i++)
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
		m_tiles[i]->SetCorridor(false);
	}
	m_roomsData.erase(m_roomsData.begin(), m_roomsData.end());
}
void World::Generate() 
{
	if (m_digPathsOneAtATime)
		GenerateLevelP1();
	else
		GenerateLevel();
}
void World::GenerateLevel()
{
	clearPreviousLevel();
	
	
	BSP bsp = BSP(m_horizontalTileCount, m_verticalTileCount);
	
	bsp.SetIgnoreExistingPaths(m_ignoreExistingPaths);
	//when width = height = x round(log(x / 3) / log(2)) + 1 = number of times to split	
	bsp.BeginSplit(round(std::log(m_horizontalTileCount / 3) / std::log(2)) + 1);//seems to produce a good ratio of rooms as long as the width = height
	//bsp.BeginSplit(4);	
	setWindowTitle();
	AStarSearch AStar = AStarSearch();	
	AStar.Initialize(GetMapDimentions(), GetTileCount(), false);
	AStar.SetWallDigCost(200);		
	BSP::TunnelingType tuntype = (BSP::TunnelingType)m_pathGenerationType;
	bsp.SetTunnelingType(tuntype);
	std::vector<std::vector<int>> rooms = bsp.GetRoomTileIndexes();
	AddRooms(rooms);
	AStar.CastTilesToAStarNodes((*this), true);
	std::vector<int> paths = bsp.GeneratePaths(AStar);
	AddPaths(paths);
	FillRoomDataStructs(&bsp);

	if (!m_playerCreated)
		m_player = CreatePlayer();	
	m_playerStart = GetPlayerStartLocation(rooms, &m_roomPlayerSpawnin);
	Tile* t = GetTileAtIndex(m_playerStart);
	m_player->SetLocation(t);
	t->SetContents(m_player);	
	//EXIT generation
	int exitIndex = CreateExit(&bsp);
	GenerateItems(exitIndex, &bsp);
	

}
void World::GenerateLevelP1() {


	m_incrementalPathDigging = 0;
	if (m_bsp != nullptr)
		delete(m_bsp);
	if (m_AStar != nullptr)
		delete(m_AStar);
	clearPreviousLevel();

	m_bsp = new BSP(m_horizontalTileCount, m_verticalTileCount);

	m_bsp->SetIgnoreExistingPaths(m_ignoreExistingPaths);
	m_bsp->BeginSplit(round(std::log(m_horizontalTileCount / 3) / std::log(2)) + 1); //seems to produce a good ratio of rooms as long as the width = height
	setWindowTitle();
	m_AStar = new AStarSearch();
	m_AStar->Initialize(GetMapDimentions(), GetTileCount(), false);
	m_AStar->SetWallDigCost(200);	
	

	BSP::TunnelingType tuntype = (BSP::TunnelingType)m_pathGenerationType;
	m_bsp->SetTunnelingType(tuntype);
	std::vector<std::vector<int>> rooms = m_bsp->GetRoomTileIndexes();
	AddRooms(rooms);
	m_AStar->CastTilesToAStarNodes((*this), true);


	
}

void World::GenerateLevelP2() 
{
	if (m_bsp != nullptr)
	{
		std::vector<int> paths = m_bsp->GeneratePathsIncremental((*m_AStar), m_incrementalPathDigging);
		AddPaths(paths);
		m_incrementalPathDigging++;
	}
	
}



int World::CreateExit(BSP* _bspToUse) {
	Tile* t = nullptr;
	if (_bspToUse != nullptr)
		t = GetTileAtIndex(_bspToUse->GenerateExitLocation(m_playerStart > 0 ? m_playerStart : rand() % GetTileCount(), m_roomPlayerSpawnin, (*this)));
	else
		t = GetTileAtIndex(m_bsp->GenerateExitLocation(m_playerStart > 0 ? m_playerStart : rand() % GetTileCount(), m_roomPlayerSpawnin, (*this)));
	
	Exit* e = new Exit(this);
	e->Init("img/Exit.bmp", "Exit", m_scene->GetRenderer());
	e->SetSize(GetTileSize().X, GetTileSize().Y);
	e->SetLocation(t);
	t->AddItem(e);
	return t->GetPositionInVector();
}

void World::GenerateKeyDoorPair(int _exitLocation, BSP* _bspToUse) {
	Tile* t = nullptr;
	std::vector<int> doorTileIndex;
	int keyTileIndex;
	Vector2 scale = GetTileSize();

	if (_bspToUse == nullptr)
		_bspToUse = m_bsp;	
	_bspToUse->GetDoorPlacement(doorTileIndex, m_roomsData, m_playerStart, _exitLocation);
	Key* k = new Key();
	for (size_t i = 0; i < doorTileIndex.size(); i++)
	{
		DoorA* d = new DoorA();
		t = GetTileAtIndex(doorTileIndex[i]);
		t->AddItem(d);
		d->SetLocation(t);
		d->Init("img/Exit.bmp", "Door", m_scene->GetRenderer());
		d->SetSize(scale);
		k->SetDoor(d);
	}

	bool validKeyLocation = false;
	//_exitLocation
		
	int exitRoomIndex = _bspToUse->RoomIndexTileIsIn(_exitLocation);
	AStarSearch Astr = AStarSearch();
	Astr.Initialize(GetMapDimentions(), GetTileCount(), false);
	Astr.CastTilesToAStarNodes((*this), false);

	do
	{
		int randomRoom = rand() % m_roomsData.size();
		if (randomRoom != exitRoomIndex)
		{
			keyTileIndex = _bspToUse->GetRandomTileInRoom(randomRoom);
			if (Astr.BeginSearch(m_playerStart, keyTileIndex, false).size() > 0)
				validKeyLocation = true;			
				validKeyLocation = true;			
		}

	} while (!validKeyLocation);


	t = GetTileAtIndex(keyTileIndex);
	t->AddItem(k);
	k->SetLocation(t);
	k->Init("img/KeyCard.bmp", "Key", m_scene->GetRenderer());
	k->SetSize(scale);
	
}
void World::GenerateItems(int _exitLocation, BSP* _bspToUse) {	
	GenerateKeyDoorPair(_exitLocation, _bspToUse);
}


void World::AddRooms(std::vector<std::vector<int>>& const _rooms) {
	m_roomsData.erase(m_roomsData.begin(), m_roomsData.end());
	
	for (size_t i = 0; i < _rooms.size(); i++)
	{
		m_roomsData.emplace_back(RoomData());
		for (size_t j = 0; j < _rooms[i].size(); j++)
		{
			GetTileAtIndex(_rooms[i][j])->SetPassable(true);			
			if(i < 8)
				GetTileAtIndex(_rooms[i][j])->changeImage("img/blank_tile" + std::to_string(i) + ".bmp");
			//start filling out the room struct
			m_roomsData[i].sm_containsTiles.push_back(_rooms[i][j]);
			m_roomsData[i].sm_region = i;
		}
				
	}
	
}

void World::FillRoomDataStructs(BSP* _bsp) {
	if (_bsp == nullptr)
		_bsp = m_bsp;
	for (size_t i = 0; i < m_roomsData.size(); i++)
	{
		if (_bsp != nullptr)
			_bsp->ExitsFromRoom(m_roomsData[i].sm_region, m_roomsData[i].sm_exitCount, m_roomsData[i].sm_regionsExitingTo, m_roomsData[i].sm_CorridorExits, (*this));
	}

	for (size_t i = 0; i < m_roomsData.size(); i++)
	{
		auto itt = m_roomsData[i].sm_regionsExitingTo.begin();
		auto ittEnd = m_roomsData[i].sm_regionsExitingTo.end();
		while (itt != ittEnd)
		{
			int index = (*itt);
			int exitCountForGivenRoom = m_roomsData[index].sm_exitCount;
			m_roomsData[i].sm_connectedness += exitCountForGivenRoom;
			itt++;
		}

	}
}

void World::AddPaths(std::vector<int>& const _paths) {

	for (size_t j = 0; j < _paths.size(); j++)
	{
		
		if (!GetTileAtIndex(_paths[j])->IsPassable())
		{
			GetTileAtIndex(_paths[j])->SetCorridor(true);
		}
			
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
	BSP bsp = BSP(1,1);
	switch (_key)
	{
	case SDLK_q:
		m_resetSeed = !m_resetSeed;
		setWindowTitle();
		printf("Reset seed set to %s\n", (m_resetSeed ? "true" : "false"));
		break;
	case SDLK_g:
		if (m_resetSeed)
			srand(1024);
		if (m_cycleGenerationType)
		{
			m_pathGenerationType++;
			if (m_pathGenerationType > 4)
				m_pathGenerationType = 0;
		}
		Generate();
		break;
	case SDLK_h:
		m_cycleGenerationType = !m_cycleGenerationType;
		setWindowTitle();
		break;
	case SDLK_1:
		m_pathGenerationType = 0;
		setWindowTitle();
		printf("Path generation type set to %s\n", bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType).c_str());
		break;
	case SDLK_2:
		m_pathGenerationType = 1;
		setWindowTitle();
		printf("Path generation type set to %s\n", bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType).c_str());
		break;
	case SDLK_3:
		m_pathGenerationType = 2;
		setWindowTitle();
		printf("Path generation type set to %s\n", bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType).c_str());
		break;
	case SDLK_4:
		m_pathGenerationType = 3;
		setWindowTitle();
		printf("Path generation type set to %s\n", bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType).c_str());
		break;
	case SDLK_5:
		m_pathGenerationType = 4;
		setWindowTitle();
		printf("Path generation type set to %s\n", bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType).c_str());
		break;
	case SDLK_u:
		m_ignoreExistingPaths = !m_ignoreExistingPaths;
		setWindowTitle();
		break;
	case SDLK_p:
		if (m_digPathsOneAtATime)
			GenerateLevelP2();
		break;
	case SDLK_i:
		if (m_digPathsOneAtATime)
		{
			if (!m_playerCreated)
				m_player = CreatePlayer();
			m_playerStart = GetPlayerStartLocation(m_bsp->GetRoomTileIndexes(), &m_roomPlayerSpawnin);
			Tile* t = GetTileAtIndex(m_playerStart);
			m_player->SetLocation(t);
			t->SetContents(m_player);
			CreateExit();
		}
			
		break;
	case SDLK_y:
		m_digPathsOneAtATime = !m_digPathsOneAtATime;
		setWindowTitle();
		break;
	case SDLK_l:
		printRoomData();
		break;
	default:
		break;
	}
}

void World::printRoomData()
{
	printf("\n");
	for (size_t i = 0; i < m_roomsData.size(); i++)
	{

		printf("Room region is %d.\n", m_roomsData[i].sm_region);
		printf("Containts tiles: ");
		auto it = m_roomsData[i].sm_containsTiles.begin();
		auto itEnd = m_roomsData[i].sm_containsTiles.end();
		while(it != itEnd)
		{
			if(it < itEnd -1)
				printf("%d, ", (*it));
			else
				printf("%d\n", (*it));
			it++;
		}
		printf("Exit count is %d\n", m_roomsData[i].sm_exitCount);
		printf("Connects to rooms: ");
		auto it2 = m_roomsData[i].sm_regionsExitingTo.begin();
		auto itEnd2 = m_roomsData[i].sm_regionsExitingTo.end();
		while (it2 != itEnd2)
		{
			printf("%d, ", (*it2));
			it2++;
		}
		printf("\n");

		auto it3 = m_roomsData[i].sm_CorridorExits.begin();
		auto itEnd3 = m_roomsData[i].sm_CorridorExits.end();
		printf("Corridor exit points(as indexes): ");
		while (it3 != itEnd3)
		{
			printf("%d, ", (*it3));
			it3++;
		}
		printf("\n");
		
		printf("Connectedness value is %d\n", m_roomsData[i].sm_connectedness);
		printf("\n\n");
	}
}
	


