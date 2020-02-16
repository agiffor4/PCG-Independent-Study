#pragma once
#include "SDL.h"
#include <memory>
#include <Vector>
#include <string>
#include "Vector2.h"
#include "IInputHandler.h"
class Tile;
class Scene;
class Vector2;
class Player;
class World : public IInputHandler
{
protected:
	std::vector<Tile*> m_tiles;
	int m_horizontalTileCount = 0;
	int m_verticalTileCount = 0;
	bool m_playerCreated = false;
	Player* m_player = nullptr;
	Scene* m_scene = nullptr;
	int m_pathGenerationType = 0;
	bool m_cycleGenerationType = false;
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
	std::vector<Tile*> GetNeighbors(Tile* _tileToFindNeighborsFor);
	std::vector<Tile*> GetNeighbors(int _tileToFindNeighborsFor);
	std::vector<Tile*> GetTiles();
	void AddTile(Tile* _renderable);
	int GetTileCount();	
	Vector2 GetTileSize();
	
	void GenerateTiles(int _screenWidth, int _screenHeight);
	void GenerateLevel();
	void clearPreviousLevel();
	

	void AddRoomsAndPaths(std::vector<std::vector<int>>& const _rooms, std::vector<int>& const _paths);
	void AddRooms(std::vector<std::vector<int>>& const _rooms);
	void AddPaths(std::vector<int>& const _paths);

	int GetPlayerStartLocation(const std::vector<std::vector<int>>& _rooms, int* roomSpawnedIn);
	Player* CreatePlayer();
	void InvokeKeyUp(SDL_Keycode _key) override;

};

