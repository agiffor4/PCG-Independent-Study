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
class RoomTree;
class Weapon;
class Enemy;
class Thing;
class World : public IInputHandler
{
private:
	SDL_Window* m_window = nullptr;
	int m_generationNumber = 0;
protected:
	
	std::vector<RoomData> m_roomsData;
	std::vector<Tile*> m_tiles;
	int m_horizontalTileCount = 0;
	int m_verticalTileCount = 0;
	int m_playerStart = -1;
	int m_roomPlayerSpawnin = -1;
	bool m_playerCreated = false;
	bool m_generateInStages = false;
	bool m_markRooms = false;
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
	void generateTreasure();
	void generateWeapon();
	void generateChests();
	void createTreasureInRoom(int _roomToCreateTreasureIn);
	void createWeaponInRoom(int _roomToCreateTreasureIn);
	int m_keyDoorGenerationType = 0;
	int m_lastKeyRoom = -1;
	int m_lastKeyDepth = -1;
	std::vector<Enemy*> m_EnemiesOnLevel;
	float m_scale = 32.0f / 64.0f;
	void tileRenderingSetUp();
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
	int GetRandomTileInRoom(int _roomIndexToGetRandomTileFrom);
	void AddTile(Tile* _renderable);
	int GetTileCount();	
	Vector2 GetTileSize();
	
	void GenerateTiles(int _screenWidth, int _screenHeight);
	void Generate();
	void GenerateLevel();
	void GenerateLevelP1();
	bool GenerateLevelP2();
	int CreateExit(BSP* _bspToUse = nullptr);
	void GenerateKeyDoorPair(int _roomToGenerateDoorsIn, RoomTree& _roomTree, std::string& _doorImage, std::string&  _keyImage, BSP* _bspToUse = nullptr);
	void GenerateDoors(int _exitLocation, int _keyDoorPairCountToGenerate, bool _ensureDoorToExit, BSP* _bspToUse = nullptr);
	void GenerateItems(int _exitLocation, BSP* _bspToUse = nullptr);

	void clearPreviousLevel();
	

	void AddRooms(std::vector<std::vector<int>>& const _rooms);
	void FillRoomDataStructs(BSP* _bsp = nullptr);
	void AddPaths(std::vector<int>& const _paths);
	int GetIndexOfRoomTileIsIn(int _index);
	int GetPlayerStartLocation(const std::vector<std::vector<int>>& _rooms, int* roomSpawnedIn);
	Player* CreatePlayer();
	Weapon* CreateWeapon();
	void PlacePlayer(std::vector<std::vector<int>>* _rooms = nullptr);
	void InvokeKeyUp(SDL_Keycode _key) override;
	void centerCameraOnPlayer(Tile* _tileToCenterOn);
	Vector2 CheckIfCameraShouldMove(Vector2 _cameraMoveDirection);
	const std::vector<Enemy*>& GetEnemiesOnLevel();
	
	Enemy* GetNearestEnemy(Vector2 _referencePoint, float _maximumRange = -1);
	std::set<Tile*> GetTilesInRadiusFromEpicenter(int _epicenter, int _radius);
	std::vector<Enemy*> GetEnemiesInTileRadius(int _epicenter, int _radius);
	std::vector<Enemy*> GetEnemiesInRadius(Vector2 _referencePoint, float _maximumRange = -1);
	Thing* GetNearestThing(Vector2 _referencePoint, const std::vector<Thing*>& _vectorToCheck, float _maximumRange = -1);
	Tile* GetNeighborNearestPoint(Vector2 _referancePoint, Tile* _tile, bool _ignoreImpassable, bool _ignoreInteractablesForPassabilityCheck);
	Tile* GetNeighborNearestPoint(Vector2 _referancePoint, int _tile, bool _ignoreImpassable, bool _ignoreInteractablesForPassabilityCheck);
	int GetLevel();
	//DEBUGGINg
	
	void printRoomData();
	
};

