#include "AStarSearch.h"
#include "Tile.h"
#include "World.h"
#include <set>
#include "AStarNode.h"

AStarSearch::AStarSearch() 
{
	
	
}
AStarSearch::~AStarSearch() {}

void AStarSearch::Initialize(Vector2 _mapDimensions, int _worldSize) {

	Vector2 size = _mapDimensions;
	m_rowCount = size.X;
	m_columnCount = size.Y;
	m_worldSize = _worldSize;
	m_inited = true;
	
}

bool AStarSearch::isValid(AStarNode& _tile)
{
	return _tile.GetPositionInVector() > -1 && _tile.GetPositionInVector() < m_worldSize;
}
bool AStarSearch::isDestination(AStarNode& _tile) {
	return _tile.GetPositionInVector() == m_targetTile->GetPositionInVector();
}

double AStarSearch::calculateHValue(AStarNode& _currentTile)
{
	// Return using the distance formula 
	return Vector2::GetSquareDistance(m_targetTile->GetPositionInGrid(), _currentTile.GetPositionInGrid());
}

std::vector<int> AStarSearch::getNodeNeighbors(int _indexToGetNeighborsFor) {

	std::vector<AStarNode*> neighborTiles = getNeighbors(_indexToGetNeighborsFor);
	std::vector<int> neighborIndexs;
	for (size_t i = 0; i < neighborTiles.size(); i++)
	{
		neighborIndexs.push_back(neighborTiles[i]->GetPositionInVector());
	}

	return neighborIndexs;
}

std::stack<int> AStarSearch::BeginSearch(int _current, int _target) {
	
	return BeginSearch((*getTileAtIndex(_current)), (*getTileAtIndex(_target)));
}
std::stack<int> AStarSearch::BeginSearch(AStarNode& _current, AStarNode& _target)
{
	if (!m_inited)
	{
		printf("Must call Initialize for AStar before searching.\n");
		return std::stack<int>();
	}
	if (m_nodes.size() < 1)
	{
		printf("Must call convert function of some kind and pass in a set of nodes before begining search.\n");
		return std::stack<int>();
	}

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
	std::fill(closedList.begin(), closedList.end(), false);

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
			if (isDestination(*getTileAtIndex(checkingNode.m_IndexOfTile)))
			{
				cn.m_Parent = &checkingNode;
				destinationFound = true;
				return findPath(nodeData);
			}
			else if (!closedList[cn.m_IndexOfTile] && getTileAtIndex(cn.m_IndexOfTile)->IsPassable())
			{
				gNew = checkingNode.gCost + getTileAtIndex(cn.m_IndexOfTile)->m_GCost;//Change the +1 to geting a value from the corisponding tile so terrain has weight
				hNew = calculateHValue(*getTileAtIndex(cn.m_IndexOfTile));
				fNew = gNew + hNew;
				if (cn.fCost == FLT_MAX || cn.fCost > fNew)
				{
					
					cn.fCost = fNew;
					cn.gCost = gNew;
					cn.hCost = hNew; 
					cn.m_Parent = &checkingNode;
					openList.insert(cn);
				}
			}
		}
		
	}
	if (destinationFound == false)
		printf("Failed to find the Destination Cell.\n");
	return empty;
}

std::stack<int> AStarSearch::findPath(std::vector<Node>& _path)
{
	
	int currentIndexToCheck = m_targetTile->GetPositionInVector();
	printf("The path is %d", currentIndexToCheck);
	std::stack<int> path;
	while (_path[currentIndexToCheck].m_IndexOfTile != _path[currentIndexToCheck].m_Parent->m_IndexOfTile)
	{
		path.push(_path[currentIndexToCheck].m_IndexOfTile);
		currentIndexToCheck = _path[currentIndexToCheck].m_Parent->m_IndexOfTile;
		printf(", %d", _path[currentIndexToCheck].m_IndexOfTile);
	}
	path.push(_path[currentIndexToCheck].m_IndexOfTile);
	printf("\n");
	return path;
}

AStarNode* AStarSearch::getAdjacentTile(int _currentTileIndex, TileDirection _direction)
{

	int targIndex = 0;
	switch (_direction)
	{
	case TileDirection::UP:
		targIndex = _currentTileIndex - m_columnCount;
		//not in top row, is passable
		if (_currentTileIndex < m_columnCount)
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::DOWN:
		targIndex = _currentTileIndex + m_columnCount;
		//not in bottom row, is passable
		if (_currentTileIndex > (m_nodes.size() - 1) - m_columnCount)
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::LEFT:
		targIndex = _currentTileIndex - 1;
		//does not carry over to previous row, is in bounds, is passible
		if (targIndex < 0 ||
			targIndex % m_columnCount == m_columnCount - 1)
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::RIGHT:
		targIndex = _currentTileIndex + 1;
		//does not carry over to next row, is in bounds, is passible
		if (targIndex % m_columnCount == 0 ||
			targIndex > m_nodes.size() - 1)
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::UPLEFT:
		targIndex = _currentTileIndex - m_columnCount - 1;
		//not in top row, in bounds, does not carry over to end of above row, is passable
		if (_currentTileIndex < m_columnCount ||
			targIndex < 0 ||
			targIndex % m_columnCount == (m_columnCount - 1))
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::UPRIGHT:
		targIndex = _currentTileIndex - m_columnCount + 1;
		//not in top row, does not carry over to current row, is passable
		if (_currentTileIndex < m_columnCount ||
			targIndex % m_columnCount == 0)
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::DOWNLEFT:
		//not in bottom row, does not carry over to end of current row, is passable
		targIndex = _currentTileIndex + m_columnCount - 1;
		if (_currentTileIndex > (m_nodes.size() - 1) - m_columnCount ||
			targIndex % m_columnCount == m_columnCount - 1)
		{
			return nullptr;
		}
		return m_nodes[targIndex];
	case TileDirection::DOWNRIGHT:
		//not in bottom row, does not carry over to beginning of next row, is passable
		targIndex = _currentTileIndex + m_columnCount + 1;
		if (_currentTileIndex > (m_nodes.size() - 1) - m_columnCount ||
			targIndex % m_columnCount == 0)
		{
			return nullptr;
		}

		return m_nodes[targIndex];
	default:
		printf("Collision check resulted in direction hitting default case.\n");
		return nullptr;
	}
}


std::vector<AStarNode*> AStarSearch::getNeighbors(int _tileToFindNeighborsFor)
{
	std::vector<AStarNode*> neighbors;
	for (size_t i = 0; i < 8; i++)
	{
		AStarNode* currentCheck = getAdjacentTile(_tileToFindNeighborsFor, (TileDirection)i);
		if (currentCheck != nullptr)
		{
			neighbors.push_back(currentCheck);
		}
	}
	return neighbors;
}

void AStarSearch::CastTilesToAStarNodes(World& _world)
{
	cleanNodes();
	for (size_t i = 0; i < _world.GetTileCount(); i++)
	{
		AStarNode* asn = new AStarNode();
		Tile* t = _world.GetTileAtIndex(i);
		asn->Init(i, t->GetPositionInGrid().X, t->GetPositionInGrid().Y);
		asn->SetPassable(t->IsPassable());
		m_nodes.push_back(asn);
	}

}
void AStarSearch::CastIntVectorToAStarNodes(std::vector<int>& _world, int _width)
{
	cleanNodes();
	for (size_t i = 0; i < _world.size(); i++)
	{
		AStarNode* asn = new AStarNode();		
		int x = (i % _width);
		asn->Init(i, x, (i - x) / _width);
		asn->SetPassable(true);
		asn->m_GCost = _world[i] > 0 ? 10 : 1;
		m_nodes.push_back(asn);
	}
}

void AStarSearch::cleanNodes() {

	for (size_t i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i] != nullptr)
		{
			delete(m_nodes[i]);
			m_nodes[i] = nullptr;
		}
	}
	m_nodes.erase(m_nodes.begin(), m_nodes.end());
}

AStarNode* AStarSearch::getTileAtIndex(int _index) {
	return _index > -1 && _index < m_nodes.size() ? m_nodes[_index] : nullptr;
}

