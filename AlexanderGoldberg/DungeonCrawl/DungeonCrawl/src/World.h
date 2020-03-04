#pragma once
#include "SDL.h"
#include <memory>
#include <vector>
#include <set>
#include <string>
#include "Vector2.h"
#include "IInputHandler.h"
#include "RoomData.h"
class Tile;
class Scene;
class Vector2;
class Player;
class BSP;
class AStarSearch;
class World : public IInputHandler
{
private:
	SDL_Window* m_window = nullptr;
protected:
	
	std::vector<RoomData> m_roomsData;
	std::vector<Tile*> m_tiles;
	int m_horizontalTileCount = 0;
	int m_verticalTileCount = 0;
	int m_playerStart = -1;
	int m_roomPlayerSpawnin = -1;
	bool m_playerCreated = false;
	bool m_generateInStages = false;
	Player* m_player = nullptr;
	Scene* m_scene = nullptr;
	int m_pathGenerationType = 0;
	bool m_cycleGenerationType = false;
	bool m_resetSeed = false;
	bool m_ignoreExistingPaths = false;
	
	int m_incrementalPathDigging = 0;
	bool m_digPathsOneAtATime = false;
	BSP* m_bsp = nullptr;
	AStarSearch* m_AStar = nullptr;
	void setWindowTitle();
	
public:

	enum class TileDirection
	{
		UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT
	};
	World(int _hTileCount, int _vTileCount, Scene* _scene);
	~World();
	bool IsViableDirectionToMoveIn(int _currentTileIndex, TileDirection _direction);
	Tile* GetAdjacentTile(int _currentTileIndex, TileDirection _direction);
	Tile* GetTileAtIndex(int _index);
	Tile* GetTileAtPosition(int _x, int _y);
	Tile* GetTile(Tile& _tile);
	Vector2 GetMapDimentions();
	void SetWindowRef(SDL_Window* _window);
	std::vector<Tile*> GetNeighbors(Tile* _tileToFindNeighborsFor, bool _getDiagonals = true);
	std::vector<Tile*> GetNeighbors(int _tileToFindNeighborsFor, bool _getDiagonals = true);
	std::vector<Tile*> GetTiles();
	void AddTile(Tile* _renderable);
	int GetTileCount();	
	Vector2 GetTileSize();
	
	void GenerateTiles(int _screenWidth, int _screenHeight);
	void Generate();
	void GenerateLevel();
	void GenerateLevelP1();
	void GenerateLevelP2();
	int CreateExit(BSP* _bspToUse = nullptr);
	void GenerateKeyDoorPair(int _roomToGenerateDoorsIn, BSP* _bspToUse = nullptr);
	void GenerateDoors(int _exitLocation, int _keyDoorPairCountToGenerate, bool _ensureDoorToExit, BSP* _bspToUse = nullptr);
	void GenerateItems(int _exitLocation, int _keyDoorPairCountToGenerate, BSP* _bspToUse = nullptr);
	void clearPreviousLevel();
	

	void AddRooms(std::vector<std::vector<int>>& const _rooms);
	void FillRoomDataStructs(BSP* _bsp = nullptr);
	void AddPaths(std::vector<int>& const _paths);

	int GetPlayerStartLocation(const std::vector<std::vector<int>>& _rooms, int* roomSpawnedIn);
	Player* CreatePlayer();
	void InvokeKeyUp(SDL_Keycode _key) override;
	
	//DEBUGGINg
	void printRoomData();

};

