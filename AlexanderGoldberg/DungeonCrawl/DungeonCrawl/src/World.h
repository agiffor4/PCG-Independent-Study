#pragma once
#include "SDL.h"
#include <memory>
#include <Vector>
#include <string>
#include "Vector2.h"
class Tile;
class World
{
protected:
	std::vector<Tile*> m_tiles;
	int m_horizontalTileCount = 0;
	int m_verticalTileCount = 0;
public:

	enum class TileDirection
	{
		UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT
	};
	World(int _width, int _height);
	~World();
	bool IsViableDirectionToMoveIn(int _currentTileIndex, TileDirection _direction);
	void AddTile(Tile* _renderable);
	int GetTileCount();
};

