#pragma once
#include <utility> 
#include <vector>
#include <stack>
#include "Node.h"
#include "Vector2.h"
#include <fstream>
#include <iostream>
//references
//https://www.geeksforgeeks.org/a-search-algorithm/
//https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h
//https://www.youtube.com/watch?v=icZj67PTFhc


class World;
class AStarNode;
class AStarSearch
{

protected:
    enum class TileDirection
    {
        UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT
    };
    /*std::string m_logFileName = "PathFindingLog.txt";
    std::fstream m_logfile;
    bool m_logPathFinding = false;*/
    int m_rowCount = -1;
    int m_columnCount = -1;
    int m_worldSize = 0;
    bool  m_inited = false;
    bool m_useDiagonals = false;
    AStarNode* m_targetTile = nullptr;
    std::vector<AStarNode*> m_nodes;
    bool isValid(AStarNode& _tile);
    bool isDestination(AStarNode& _tile);
    double calculateHValue(AStarNode& _currentTile);
    std::vector<int> getNodeNeighbors(int _indexToGetNeighborsFor);
    AStarNode* getAdjacentTile(int _currentTileIndex, TileDirection _direction);
    std::vector<AStarNode*> getNeighbors(int _tileToFindNeighborsFor);
    std::stack<int> findPath(std::vector<Node>& _path);
    void cleanNodes();
    AStarNode* getTileAtIndex(int _index);
    
public:
    
	AStarSearch();
	~AStarSearch();
    void Initialize(Vector2 _mapDimensions, int _worldSize, bool _useDiagonals);
    std::stack<int> BeginSearch(int _current, int _target);
    std::stack<int> BeginSearch(AStarNode& _current, AStarNode& _target);
    
    
    void CastTilesToAStarNodes(World& _world);
    void CastIntVectorToAStarNodes(std::vector<int>& _world, int _width);
    
    
    
  


    
};