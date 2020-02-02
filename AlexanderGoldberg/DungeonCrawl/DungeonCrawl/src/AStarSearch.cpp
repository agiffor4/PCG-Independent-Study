#include "AStarSearch.h"
#include "Tile.h"
#include "World.h"
#include <set>


AStarSearch::AStarSearch() 
{
	
	
}
AStarSearch::~AStarSearch() {}

void AStarSearch::Initialize(World& _world) {

	Vector2 size = _world.GetMapDimentions();
	m_rowCount = size.X;
	m_columnCount = size.Y;
	m_worldSize = _world.GetTileCount();
	m_world = &_world;
}

bool AStarSearch::isValid(Tile& _tile)
{
	return _tile.GetPositionInVector() > -1 && _tile.GetPositionInVector() < m_worldSize;
}
bool AStarSearch::isDestination(Tile& _tile) {
	return _tile.GetPositionInVector() == m_targetTile->GetPositionInVector();
}

double AStarSearch::calculateHValue(Tile& _currentTile)
{
	// Return using the distance formula 
	return Vector2::GetSquareDistance(m_targetTile->GetPositionInGrid(), _currentTile.GetPositionInGrid());
}

std::vector<int> AStarSearch::getNodeNeighbors(int _indexToGetNeighborsFor) {

	std::vector<Tile*> neighborTiles = m_world->GetNeighbors(_indexToGetNeighborsFor);
	std::vector<int> neighborIndexs;
	for (size_t i = 0; i < neighborTiles.size(); i++)
	{
		neighborIndexs.push_back(neighborTiles[i]->GetPositionInVector());
	}

	return neighborIndexs;
}

std::stack<int> AStarSearch::AStar(Tile& _current, Tile& _target)
{
	int startIndex = _current.GetPositionInVector();
	std::stack<int> empty;
	if (!isValid(_target))
	{
		printf("Chosen destination is invalid.\n");
		return empty;
	}
	else
	{
		m_targetTile = &_target;
	}
	if (!isValid(_current))
	{
		printf("current placement is invalid.\n");
		return empty;
	}
	if (isDestination(_current))
	{
		printf("Already at destination.\n");
		return empty;
	}


	std::vector<bool> closedList;
	closedList.resize(m_worldSize);
	//std::fill(closedList.begin(), closedList.end(), false);

	std::vector<Node> nodeData;
	
	for (size_t i = 0; i < m_worldSize; i++)
	{
		nodeData.push_back(Node());
		nodeData[i].m_IndexOfTile = i;
	}
		

	nodeData[startIndex].gCost = 0;
	nodeData[startIndex].hCost = 0;
	nodeData[startIndex].fCost = 0;
	nodeData[startIndex].m_Parent = &nodeData[startIndex];


	std::set<Node> openList;
	
	openList.insert(nodeData[startIndex]);
	bool destinationFound = false;


	while (!openList.empty())
	{
		Node checkingNode = (*openList.begin());
		openList.erase(openList.begin());
		closedList[checkingNode.m_IndexOfTile] = true;
		float gNew = 0; 
		float hNew = 0;
		float fNew = 0;
		std::vector<int> NodeNeighborIndexes = getNodeNeighbors(checkingNode.m_IndexOfTile);
		std::vector<Node> neighbors;
		for (size_t i = 0; i < NodeNeighborIndexes.size(); i++)
		{
			neighbors.push_back(nodeData[NodeNeighborIndexes[i]]);
		}


		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node cn = neighbors[i]; //current node
			if (isDestination(*m_world->GetTileAtIndex(checkingNode.m_IndexOfTile)))
			{
				cn.m_Parent = &checkingNode;
				destinationFound = true;
				return FindPath(nodeData);
			}
			else if (!closedList[cn.m_IndexOfTile] && m_world->GetTileAtIndex(cn.m_IndexOfTile)->IsPassable())
			{
				gNew = cn.gCost + 1.0;//Change the +1 to geting a value from the corisponding tile so terrain has weight
				hNew = calculateHValue(*m_world->GetTileAtIndex(cn.m_IndexOfTile));
				fNew = gNew + hNew;
				if (cn.fCost == FLT_MAX || cn.fCost > fNew)
				{
					openList.insert(cn);
					cn.fCost = fNew;
					cn.gCost = hNew;
					cn.hCost = gNew; 
					cn.m_Parent = &checkingNode;
				}
			}
		}
		
	}
	if (destinationFound == false)
		printf("Failed to find the Destination Cell.\n");
	return empty;
}

std::stack<int> AStarSearch::FindPath(std::vector<Node>& _path)
{
	printf("The path is ");
	int currentIndexToCheck = m_targetTile->GetPositionInVector();
	std::stack<int> path;
	while (_path[currentIndexToCheck].m_IndexOfTile != _path[currentIndexToCheck].m_Parent->m_IndexOfTile)
	{
		path.push(_path[currentIndexToCheck].m_IndexOfTile);
		currentIndexToCheck = _path[currentIndexToCheck].m_Parent->m_IndexOfTile;
	}
	path.push(_path[currentIndexToCheck].m_IndexOfTile);
	
	return path;
}






