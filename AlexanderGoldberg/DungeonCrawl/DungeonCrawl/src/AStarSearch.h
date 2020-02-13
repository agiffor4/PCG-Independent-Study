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
    bool m_allowPathsAlongEdges = false;
    int m_DigCost = 200;
    AStarNode* m_targetTile = nullptr;
    std::vector<AStarNode*> m_nodes;
    std::vector<AStarNode*> m_modifableNodes;
    bool isValid(AStarNode& _tile);
    bool isDestination(AStarNode& _tile);
    double calculateHValue(AStarNode& _currentTile);
    std::vector<int> getNodeNeighbors(int _indexToGetNeighborsFor, std::vector<AStarNode*>& _nodesToUse);
    AStarNode* getAdjacentTile(int _currentTileIndex, TileDirection _direction, std::vector<AStarNode*>& _nodesToUse);
    std::vector<AStarNode*> getNeighbors(int _tileToFindNeighborsFor, std::vector<AStarNode*>& _nodesToUse);
    bool isTileEdge(int _index);
    std::stack<int> findPath(std::vector<Node>& _path);
    void cleanNodes();
    void cleanModifiableNodes();
    AStarNode* getTileAtIndex(int _index, std::vector<AStarNode*>& _nodesToUse);
    
public:
    
	AStarSearch();
	~AStarSearch();
    void Initialize(Vector2 _mapDimensions, int _worldSize, bool _useDiagonals);
    std::stack<int> BeginSearch(int _current, int _target, bool _usePreviousPathsInEachItteration);
    std::stack<int> BeginSearch(AStarNode& _current, AStarNode& _target, bool _usePreviousPathsInEachItteration);
    void SetWallDigCost(int _newCost);
    
    void CastTilesToAStarNodes(World& _world);
    void CastIntVectorToAStarNodes(std::vector<int>& _world, int _width);
    
    
    
  


    
};