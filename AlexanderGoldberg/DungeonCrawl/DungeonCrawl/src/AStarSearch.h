#pragma once
#include <utility> 
#include <vector>
#include <stack>
#include "Node.h"
#include "Vector2.h"
//references
//https://www.geeksforgeeks.org/a-search-algorithm/
//https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h
//https://www.youtube.com/watch?v=icZj67PTFhc

class Tile;
class World;
class AStarSearch
{

protected:
    
    World* m_world = nullptr;
    int m_rowCount = -1;
    int m_columnCount = -1;
    int m_worldSize = 0;
    Tile* m_targetTile = nullptr;
    bool isValid(Tile& _tile);
    bool isDestination(Tile& _tile);
    double calculateHValue(Tile& _currentTile);
    std::vector<int> getNodeNeighbors(int _indexToGetNeighborsFor);
public:
    
	AStarSearch();
	~AStarSearch();
    void Initialize(World& _world);
    std::stack<int> AStar(Tile& _current, Tile& _target);
    std::stack<int> FindPath(std::vector<Node>& _path);

    
  


    
};