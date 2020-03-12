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
#include "RoomTree.h"
#include "Treasure.h"
#include "Camera.h"
#define Debug = 1;

std::string numToColor[] = { "Red", "Blue", "Yellow"};
void World::setWindowTitle()
{
	if (m_window != nullptr)
	{
		BSP bsp = BSP(1, 1);
		std::string width = "w: " + std::to_string(m_horizontalTileCount) + " ";
		std::string height = "h: " + std::to_string(m_verticalTileCount) + " ";
		std::string tunnelT = "Path: " + bsp.GetEnumName((BSP::TunnelingType)m_pathGenerationType) + " ";		
		std::string seed = " seed Reset: " + std::string(m_resetSeed ? "T" : "F") + " ";
		std::string cycle = " Cycle:  " + std::string(m_cycleGenerationType ? "T" : "F") + " ";
		std::string ignore = " carve New: " + std::string(m_ignoreExistingPaths ? "T" : "F") + " ";		
		std::string incremental = " Increment dig: " + std::string(m_digPathsOneAtATime ? "T" : "F") + " ";
		std::string markRooms = " Marks: " + std::string(m_markRooms ? "T" : "F");
		std::string level = std::to_string(m_generationNumber);
		std::string title = width + height + tunnelT + seed + cycle + ignore + incremental + markRooms + " L: " + level;
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
	
#if UseCamera == 1
	Vector2 targetSize = Vector2(32, 32);
#else
	Vector2 targetSize = Vector2(_screenWidth / m_horizontalTileCount, _screenHeight / m_verticalTileCount);
#endif

	for (size_t i = 0; i < m_verticalTileCount; i++)
	{
		for (size_t j = 0; j < m_horizontalTileCount; j++)
		{
			std::string name = "Tile (" + std::to_string(j) + ", " + std::to_string(i) + ")";
			Tile* t = new Tile();
			//t->Init("img/blank_tile.bmp", name, GetTileCount(), j, i, Vector2(j * targetSize.X, i * targetSize.X), m_scene->GetRenderer());		
			t->Init("img/blank_tile.bmp", name, GetTileCount(), j, i, Vector2(j * m_scale, i * m_scale), m_scene->GetRenderer());
			t->SetPassable(false);
			//t->SetSize(targetSize);
			t->SetScale(m_scale);
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
	m_generationNumber++;
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
	setWindowTitle();
	AStarSearch AStar = AStarSearch();	
	AStar.Initialize(GetMapDimentions(), GetTileCount(), false);
	AStar.SetWallDigCost(200);		
	BSP::TunnelingType tuntype = (BSP::TunnelingType)m_pathGenerationType;
	bsp.SetTunnelingType(tuntype);	
	std::vector<std::vector<int>> rooms;

#ifdef Debug
	int timesRound = 100;
#endif
	do
	{	
		//when width = height = x round(log(x / 3) / log(2)) + 1 = number of times to split	
		bsp.BeginSplit(round(std::log(m_horizontalTileCount / 3) / std::log(2)) + 1);//seems to produce a good ratio of rooms as long as the width = height	
		rooms = bsp.GetRoomTileIndexes();
#ifdef Debug
		timesRound--;
		if (timesRound < 0)
		{
			break;
		}
#endif
	} while (rooms.size() < 1);
	AddRooms(rooms);
	AStar.CastTilesToAStarNodes((*this), true);
	std::vector<int> paths = bsp.GeneratePaths(AStar);
	AddPaths(paths);
	FillRoomDataStructs(&bsp);

	if (!m_playerCreated)
		m_player = CreatePlayer();
	PlacePlayer(&rooms);
	
	
#if UseCamera == 1
	Vector2 offset = t->GetPosition();	
	offset.X *= 0.5f;
	offset.Y *= 0.5f;
	Camera::SetOffset(offset);
	Vector2 center = Vector2(320, 240);
	Camera::SetCenter(center);
#endif
	//EXIT generation
	int exitIndex = CreateExit(&bsp);
	GenerateDoors(exitIndex, 2, true, &bsp);
	GenerateItems(exitIndex, &bsp);
	
	tileRenderingSetUp();
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

bool World::GenerateLevelP2() 
{
	bool result = false;
	if (m_bsp != nullptr)
	{
		std::vector<int> paths = m_bsp->GeneratePathsIncremental((*m_AStar), m_incrementalPathDigging, result);
		AddPaths(paths);
		m_incrementalPathDigging++;
	}
	return result;
}



int World::CreateExit(BSP* _bspToUse) {
	Tile* t = nullptr;
	if (_bspToUse != nullptr)
		t = GetTileAtIndex(_bspToUse->GenerateExitLocation(m_playerStart > 0 ? m_playerStart : rand() % GetTileCount(), m_roomPlayerSpawnin, (*this)));
	else
		t = GetTileAtIndex(m_bsp->GenerateExitLocation(m_playerStart > 0 ? m_playerStart : rand() % GetTileCount(), m_roomPlayerSpawnin, (*this)));
	
	Exit* e = new Exit(this);
	e->Init("img/Exit.bmp", "Exit", m_scene->GetRenderer());
	//e->SetSize(GetTileSize().X, GetTileSize().Y);
	e->SetScale(m_scale);
	e->SetLocation(t);
	t->AddItem(e);
	return t->GetPositionInVector();
}

void World::GenerateKeyDoorPair(int _roomToGenerateDoorsIn, RoomTree& _roomTree, std::string& _doorImage, std::string& _keyImage, BSP* _bspToUse) {
	Tile* t = nullptr;
	std::vector<int> doorTileIndex;
	int keyTileIndex = 0;
	Vector2 scale = GetTileSize();


	if (_bspToUse == nullptr)
		_bspToUse = m_bsp;	

	_bspToUse->GetDoorPlacement(doorTileIndex, m_roomsData, m_playerStart, _roomToGenerateDoorsIn);
	_roomTree.GenerateRoomTree(m_roomsData, _bspToUse->RoomIndexTileIsIn(m_playerStart));

	std::vector<int> roomsBlockedOff = _roomTree.StartLockRooms(_roomToGenerateDoorsIn);// find which other rooms are no longer accesable

	for (size_t i = 0; i < roomsBlockedOff.size(); i++)
		m_roomsData[roomsBlockedOff[i]].sm_Locked = true; //mark those rooms as locked
	

	Key* k = new Key();
	for (size_t i = 0; i < doorTileIndex.size(); i++)
	{
		DoorA* d = new DoorA();
		t = GetTileAtIndex(doorTileIndex[i]);
		t->AddItem(d);
		d->SetLocation(t);
		d->Init(_doorImage, "Door", m_scene->GetRenderer());
		//d->SetSize(scale);
		d->SetScale(m_scale);
		k->SetDoor(d);
	}

	bool validKeyLocation = false;

	if (m_lastKeyDepth == -1)
		m_lastKeyDepth = _roomTree.GetDeepestDepth();

	switch (m_keyDoorGenerationType)
	{
	case 0:
		do
		{
			int randomRoom = rand() % m_roomsData.size();
			if (randomRoom != _roomToGenerateDoorsIn)
			{
				if (!m_roomsData[randomRoom].sm_Locked)
				{
					keyTileIndex = _bspToUse->GetRandomTileInRoom(randomRoom);
					validKeyLocation = true;
				}
			}
		} while (!validKeyLocation);
		break;
	case 1:
		do
		{
			int randomRoom = rand() % m_roomsData.size();
			if (randomRoom != _roomToGenerateDoorsIn)
			{
				int depthToCheckFor = m_lastKeyDepth >= _roomTree.GetDeepestDepth()-1 ? m_lastKeyDepth - 3 : (m_lastKeyDepth > 2 ? m_lastKeyDepth - 2 : m_lastKeyDepth);
				if (!m_roomsData[randomRoom].sm_Locked 
					&& _roomTree.IsRoomDepthGreaterOrEqual(randomRoom, depthToCheckFor))
				{
					keyTileIndex = _bspToUse->GetRandomTileInRoom(randomRoom);
					m_lastKeyDepth = _roomTree.GetRoomDepth(randomRoom);
					if (m_lastKeyDepth < 0)
						m_lastKeyDepth = 0;					
					validKeyLocation = true;
					m_lastKeyRoom = randomRoom;
				}
			}
		} while (!validKeyLocation);
		break;
	default:
		
		break;
	}
	t = GetTileAtIndex(keyTileIndex);
	t->AddItem(k);
	k->SetLocation(t);
	k->Init(_keyImage, "Key", m_scene->GetRenderer());
	//k->SetSize(scale);
	k->SetScale(m_scale);
	
}
void World::GenerateDoors(int _exitLocation, int _keyDoorPairCountToGenerate, bool _ensureDoorToExit, BSP* _bspToUse)
{
	
	std::vector<std::vector<int>> rooms;
	for (size_t i = 0; i < m_roomsData.size(); i++)
		rooms.push_back(m_roomsData[i].sm_containsTiles);
	int exitRoomIndex = _bspToUse->RoomIndexTileIsIn(_exitLocation, &rooms);
	int playerStartRoomIndex = _bspToUse->RoomIndexTileIsIn(m_playerStart);
	RoomTree roomTree = RoomTree();
	std::string doorPath = "img/ExitDoor.bmp";
	std::string keyPath = "img/Keycard.bmp";	
	if (_ensureDoorToExit)
		GenerateKeyDoorPair(exitRoomIndex, roomTree, doorPath, keyPath, _bspToUse);
	m_lastKeyDepth = roomTree.GetDeepestDepth();
	
	switch (m_keyDoorGenerationType)
	{
	case 0:
		for (size_t i = 0; i < _keyDoorPairCountToGenerate; i++)
		{
			int roomToLock = 0;
			if (m_roomsData.size() > 2)
			{
				do
				{
					roomToLock = rand() % m_roomsData.size();
				} while (roomToLock == exitRoomIndex || roomToLock == playerStartRoomIndex);
			}
			//m_lastDoorDepth = roomTree.GetRoomDepth(roomToLock);
			doorPath = "img/" + numToColor[i] + "Door.bmp";
			keyPath = "img/" + numToColor[i] + "KeyCard.bmp";
			GenerateKeyDoorPair(roomToLock, roomTree, doorPath, keyPath, _bspToUse);
		}
		break;
	case 1:
		for (size_t i = 0; i < _keyDoorPairCountToGenerate; i++)
		{
			int roomToLock = 0;
			if (m_roomsData.size() > 2)
			{
				do
				{
					roomToLock = roomTree.GetRandomParentWithinRange((m_lastKeyDepth > 4 ? 2 : m_lastKeyDepth > 3 ? 1 : 0), m_lastKeyRoom);
				} while (roomToLock == exitRoomIndex || roomToLock == playerStartRoomIndex);
			}
			doorPath = "img/" + numToColor[i] + "Door.bmp";
			keyPath = "img/" + numToColor[i] + "KeyCard.bmp";
			GenerateKeyDoorPair(roomToLock, roomTree, doorPath, keyPath, _bspToUse);
		}
	default:
		break;
	}

}
void World::GenerateItems(int _exitLocation, BSP* _bspToUse) {
	
	generateTreasure();
}

void World::generateTreasure() {	
	for (size_t i = 0; i < m_roomsData.size(); i++)
	{
		if (m_roomsData[i].sm_connectedness < 4)
		{			
			if (rand() % 2 == 0)
			{
				createTreasureInRoom(m_roomsData[i].sm_region);
			}
		}
	}
}

void World::createTreasureInRoom(int _roomToCreateTreasureIn) 
{
	#define containedTiles m_roomsData[_roomToCreateTreasureIn].sm_containsTiles
	int randomTile = containedTiles[rand() % containedTiles.size()];
	Treasure* t = new Treasure();
	t->SetLocation(m_tiles[randomTile]);
	t->Init("img/Treasure.bmp", "Treasue", m_scene->GetRenderer());	
	Vector2 scale = GetTileSize();
	//t->SetSize(scale);
	t->SetScale(m_scale);
	m_tiles[randomTile]->AddItem(t);
}

void World::tileRenderingSetUp()
{
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i]->DetermineTileType(this);
	}
}

void World::AddRooms(std::vector<std::vector<int>>& const _rooms) {
	m_roomsData.erase(m_roomsData.begin(), m_roomsData.end());
	
	for (size_t i = 0; i < _rooms.size(); i++)
	{
		m_roomsData.emplace_back(RoomData());
		for (size_t j = 0; j < _rooms[i].size(); j++)
		{
			GetTileAtIndex(_rooms[i][j])->SetPassable(true);	
			GetTileAtIndex(_rooms[i][j])->AddRoomNumber(i, m_markRooms, m_scene->GetRenderer());
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

int World::GetIndexOfRoomTileIsIn(int _index)
{
	for (size_t i = 0; i < m_roomsData.size(); i++)
	{
		if (m_roomsData[i].Contains(_index))
		{
			return i;
		}
	}
	return -1;
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
		p->SetScale(m_scale);
		return p;
	
}

void World::PlacePlayer(std::vector<std::vector<int>>* _rooms)
{
	m_playerStart = GetPlayerStartLocation(_rooms == nullptr ? m_bsp->GetRoomTileIndexes() : (*_rooms), &m_roomPlayerSpawnin);
	Tile* t = GetTileAtIndex(m_playerStart);
	m_player->SetLocation(t);
	t->SetContents(m_player);
}

void World::InvokeKeyUp(SDL_Keycode _key)
{
	BSP bsp = BSP(1,1);
	switch (_key)
	{
	case SDLK_r:
		m_resetSeed = !m_resetSeed;
		setWindowTitle();
		printf("Reset seed set to %s\n", (m_resetSeed ? "true" : "false"));
		break;
	case SDLK_g:
		if (m_resetSeed)
		{
			srand(1024);
			m_generationNumber = 0;
		}
		if (m_cycleGenerationType)
		{
			m_pathGenerationType++;
			if (m_pathGenerationType > 4)
				m_pathGenerationType = 0;
		}
		Generate();
		break;
	case SDLK_l:
		printRoomData();
		break;
	case SDLK_c:
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
	case SDLK_o:
		if (m_digPathsOneAtATime)
		{
			if (GenerateLevelP2())
			{
				if (!m_playerCreated)
					m_player = CreatePlayer();
				PlacePlayer();
				CreateExit();
				tileRenderingSetUp();
			}
		}
			
		break;

	case SDLK_i:
		m_digPathsOneAtATime = !m_digPathsOneAtATime;
		setWindowTitle();
		break;
	case SDLK_n:
		m_ignoreExistingPaths = !m_ignoreExistingPaths;
		setWindowTitle();
		break;
	case SDLK_m:
		m_markRooms = !m_markRooms;
		setWindowTitle();
		break;
		
	default:
		break;
	}
}

Vector2 World::CheckIfCameraShouldMove(Vector2 _cameraMoveDirection)
{
	Vector2 tileSize = m_tiles[0]->GetCurrentSize();
#define lastT m_tiles[m_tiles.size() - 1]
#define startT m_tiles[0]
	
	if (_cameraMoveDirection.Y == 1)
	{
		
		if (startT->GetDestination().y - Camera::Offset().Y == 0 ||
			m_player->GetLocation()->GetDestination().y - Camera::Offset().Y > Camera::GetCenter().Y - tileSize.Y
			)
		{
			_cameraMoveDirection.Y = 0;
		}
	}
		

	if (_cameraMoveDirection.Y == -1)
	{
		
		if (lastT->GetDestination().y - Camera::Offset().Y == Camera::GetScreenSize().Y - tileSize.Y || 
			m_player->GetLocation()->GetDestination().y < Camera::GetCenter().Y
			)
		{
			_cameraMoveDirection.Y = 0;
		}
	}
		
	//left
	if (_cameraMoveDirection.X == -1)
	{				
		if (startT->GetDestination().x - Camera::Offset().X == 0 ||
			m_player->GetLocation()->GetDestination().x > lastT->GetDestination().x - Camera::GetCenter().X
			)
		{
			_cameraMoveDirection.X = 0;
		}
	}
		
	//right
	if (_cameraMoveDirection.X == 1)
	{
		if (lastT->GetDestination().x - Camera::Offset().X == Camera::GetScreenSize().X - 32 ||
			m_player->GetLocation()->GetDestination().x < Camera::GetCenter().X
			)
		{
			_cameraMoveDirection.X = 0;
		}
	}
		

	return _cameraMoveDirection;
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
		printf("Room is %s\n", (m_roomsData[i].sm_Locked ? "inaccessible" : "accessible"));
		printf("\n\n");
	}
}
	


