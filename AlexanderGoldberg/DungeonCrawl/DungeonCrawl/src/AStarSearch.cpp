#include "AStarSearch.h"
#include "Tile.h"
#include "World.h"
#include <set>
#include "AStarNode.h"
AStarSearch::AStarSearch() 
{
	
	
}
AStarSearch::~AStarSearch() 
{
	cleanNodes();
	cleanModifiableNodes();
}

void AStarSearch::Initialize(Vector2 _mapDimensions, int _worldSize, bool _useDiagonals) {
	ResetAStar();
	Vector2 size = _mapDimensions;
	m_rowCount = size.X;
	m_columnCount = size.Y;
	m_worldSize = _worldSize;
	m_inited = true;
	m_useDiagonals = _useDiagonals;
}

bool AStarSearch::isValid(AStarNode& _tile)
{
	return (&_tile != nullptr) && (_tile.GetPositionInVector() > -1 && _tile.GetPositionInVector() < m_worldSize);
}
bool AStarSearch::isDestination(AStarNode& _tile) {
	return _tile.GetPositionInVector() == m_targetTile->GetPositionInVector();
}

double AStarSearch::calculateHValue(AStarNode& _currentTile)
{
	// Return using the distance formula 
	return Vector2::GetSquareDistance(m_targetTile->GetPositionInGrid(), _currentTile.GetPositionInGrid());
}

std::vector<int> AStarSearch::getNodeNeighbors(int _indexToGetNeighborsFor, std::vector<AStarNode*>& _nodesToUse) {

	std::vector<AStarNode*> neighborTiles = getNeighbors(_indexToGetNeighborsFor, _nodesToUse);
	std::vector<int> neighborIndexs;
	for (size_t i = 0; i < neighborTiles.size(); i++)
	{
		neighborIndexs.push_back(neighborTiles[i]->GetPositionInVector());
	}

	return neighborIndexs;
}

std::stack<int> AStarSearch::BeginSearch(int _current, int _target, bool _usePreviousPathsInEachItteration) {
	std::vector<AStarNode*>& refToNodeVector = (_usePreviousPathsInEachItteration ? m_modifableNodes : m_nodes);	
	return BeginSearch((*getTileAtIndex(_current, refToNodeVector)), (*getTileAtIndex(_target, refToNodeVector)), _usePreviousPathsInEachItteration);
}
std::stack<int> AStarSearch::BeginSearch(AStarNode& _current, AStarNode& _target, bool _usePreviousPathsInEachItteration)
{
	std::vector<AStarNode*>& refToNodeVector = (_usePreviousPathsInEachItteration ? m_modifableNodes : m_nodes);
	if (!m_inited)
	{
		printf("Must call Initialize for AStar before searching.\n");
		return std::stack<int>();
	}
	if (refToNodeVector.size() < 1)
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

	auto NodeComparator = [](const Node* lhs, const Node* rhs) {return lhs->fCost < rhs->fCost; };
	std::multiset<Node*, decltype(NodeComparator)> openList(NodeComparator);
	openList.emplace(&nodeData[startIndex]);
	//std::vector<Node*> openList;	
	//openList.push_back(&nodeData[startIndex]);	
	bool destinationFound = false;
	
	while (!openList.empty())
	{
		Node& checkingNode = (*(*openList.begin())); //gets next node that is available to be checked

		openList.erase(openList.begin()); 

		closedList[checkingNode.m_IndexOfTile] = true; //sets give node as having been the origin for checks
		
		float gNew = 0; 
		float hNew = 0;
		float fNew = 0;
		std::vector<int> NodeNeighborIndexes = getNodeNeighbors(checkingNode.m_IndexOfTile, refToNodeVector);
		std::vector<Node> neighbors;
		for (size_t i = 0; i < NodeNeighborIndexes.size(); i++)
		{			
			neighbors.push_back(nodeData[NodeNeighborIndexes[i]]);
		}
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node& cn = nodeData[neighbors[i].m_IndexOfTile]; //current node
			bool isPass = getTileAtIndex(cn.m_IndexOfTile, refToNodeVector)->IsPassable();
			if (isDestination(*getTileAtIndex(cn.m_IndexOfTile, refToNodeVector)))
			{
				cn.m_Parent = &checkingNode;
				destinationFound = true;
				std::stack<int> path = findPath(nodeData);
				
				if (_usePreviousPathsInEachItteration)
				{
					std::stack<int> path2 = path;
					int size = path.size();
					for (size_t i = 0; i < size; i++)
					{
						m_modifableNodes[path2.top()]->SetPassable(true);
						m_modifableNodes[path2.top()]->SetCorridor(true);
						m_modifableNodes[path2.top()]->m_GCost = 1;						
						path2.pop();
					}
					
				}
				return path;
				//return findPath(nodeData);
			}			
			else if (!closedList[cn.m_IndexOfTile] && isPass && (m_allowPathsAlongEdges ? true : !isTileEdge(cn.m_IndexOfTile)))
			{
				gNew = checkingNode.gCost + getTileAtIndex(cn.m_IndexOfTile, refToNodeVector)->m_GCost;
				hNew = calculateHValue(*getTileAtIndex(cn.m_IndexOfTile, refToNodeVector));
				fNew = gNew + hNew;
				if ((cn.fCost == FLT_MAX || cn.fCost > fNew))
				{
					cn.fCost = fNew;
					cn.gCost = gNew;
					cn.hCost = hNew; 
					cn.m_Parent = &checkingNode;
					openList.emplace(&cn);					
					//openList.push_back(&cn);
				}
			}
		}
		
	}
	if (destinationFound == false)
	{	
		printf("Failed to find the Destination Cell.\n");
	}
		
	return empty;
}

void AStarSearch::SetWallDigCost(int _newCost) 
{
	m_DigCost = _newCost;
}

void AStarSearch::SetEmptyTileCost(int _newCost)
{
	m_emptyTileGCost = _newCost;
}

void AStarSearch::ResetAStar()
{
	cleanNodes();
	cleanModifiableNodes();
}

std::stack<int> AStarSearch::findPath(std::vector<Node>& _path)
{
	
	int currentIndexToCheck = m_targetTile->GetPositionInVector();

	int x = (_path[currentIndexToCheck].m_IndexOfTile % m_columnCount);
	int y = m_rowCount - (_path[currentIndexToCheck].m_IndexOfTile - x) / m_columnCount;
	//printf("The path is %d (%d, %d)", currentIndexToCheck, x, y);
	std::stack<int> path;
	while (_path[currentIndexToCheck].m_IndexOfTile != _path[currentIndexToCheck].m_Parent->m_IndexOfTile)
	{
		path.push(_path[currentIndexToCheck].m_IndexOfTile);
		currentIndexToCheck = _path[currentIndexToCheck].m_Parent->m_IndexOfTile;
		x = (_path[currentIndexToCheck].m_IndexOfTile % m_columnCount);
		y = m_columnCount - (_path[currentIndexToCheck].m_IndexOfTile - x) / m_columnCount;
		//printf(", %d (%d, %d)", _path[currentIndexToCheck].m_IndexOfTile, x, y);

	}
	path.push(_path[currentIndexToCheck].m_IndexOfTile);
	//printf("\n");
	return path;
}

AStarNode* AStarSearch::getAdjacentTile(int _currentTileIndex, TileDirection _direction, std::vector<AStarNode*>& _nodesToUse)
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
		return _nodesToUse[targIndex];
	case TileDirection::DOWN:
		targIndex = _currentTileIndex + m_columnCount;
		//not in bottom row, is passable
		if (_currentTileIndex > (_nodesToUse.size() - 1) - m_columnCount)
		{
			return nullptr;
		}
		return _nodesToUse[targIndex];
	case TileDirection::LEFT:
		targIndex = _currentTileIndex - 1;
		//does not carry over to previous row, is in bounds, is passible
		if (targIndex < 0 ||
			targIndex % m_columnCount == m_columnCount - 1)
		{
			return nullptr;
		}
		return _nodesToUse[targIndex];
	case TileDirection::RIGHT:
		targIndex = _currentTileIndex + 1;
		//does not carry over to next row, is in bounds, is passible
		if (targIndex % m_columnCount == 0 ||
			targIndex > _nodesToUse.size() - 1)
		{
			return nullptr;
		}
		return _nodesToUse[targIndex];
	case TileDirection::UPLEFT:
		targIndex = _currentTileIndex - m_columnCount - 1;
		//not in top row, in bounds, does not carry over to end of above row, is passable
		if (_currentTileIndex < m_columnCount ||
			targIndex < 0 ||
			targIndex % m_columnCount == (m_columnCount - 1))
		{
			return nullptr;
		}
		return _nodesToUse[targIndex];
	case TileDirection::UPRIGHT:
		targIndex = _currentTileIndex - m_columnCount + 1;
		//not in top row, does not carry over to current row, is passable
		if (_currentTileIndex < m_columnCount ||
			targIndex % m_columnCount == 0)
		{
			return nullptr;
		}
		return _nodesToUse[targIndex];
	case TileDirection::DOWNLEFT:
		//not in bottom row, does not carry over to end of current row, is passable
		targIndex = _currentTileIndex + m_columnCount - 1;
		if (_currentTileIndex > (_nodesToUse.size() - 1) - m_columnCount ||
			targIndex % m_columnCount == m_columnCount - 1)
		{
			return nullptr;
		}
		return _nodesToUse[targIndex];
	case TileDirection::DOWNRIGHT:
		//not in bottom row, does not carry over to beginning of next row, is passable
		targIndex = _currentTileIndex + m_columnCount + 1;
		if (_currentTileIndex > (_nodesToUse.size() - 1) - m_columnCount ||
			targIndex % m_columnCount == 0)
		{
			return nullptr;
		}

		return _nodesToUse[targIndex];
	default:
		printf("A Star tile direction state is Unknown state value is %d.\n", (int)_direction);
		return nullptr;
	}
}


std::vector<AStarNode*> AStarSearch::getNeighbors(int _tileToFindNeighborsFor, std::vector<AStarNode*>& _nodesToUse)
{
	std::vector<AStarNode*> neighbors;
	int numberOfNeighborsToFind = m_useDiagonals ? 8 : 4;
	for (size_t i = 0; i < numberOfNeighborsToFind; i++)
	{
		AStarNode* currentCheck = getAdjacentTile(_tileToFindNeighborsFor, (TileDirection)i, _nodesToUse);
		if (currentCheck != nullptr)
		{
			neighbors.push_back(currentCheck);
		}
	}
	return neighbors;
}

bool AStarSearch::isTileEdge(int _index) {
	return _index < m_columnCount ||
		_index > m_worldSize - m_columnCount ||
		_index % m_columnCount == 0 ||
		_index % m_columnCount == m_columnCount - 1;
		
}





void AStarSearch::CastTilesToAStarNodes(World& _world, bool _canGoTroughWalls)
{
	cleanNodes();
	for (size_t i = 0; i < _world.GetTileCount(); i++)
	{
		
		AStarNode* asn = new AStarNode();
		Tile* t = _world.GetTileAtIndex(i);
		asn->Init(i, t->GetPositionInGrid().X, t->GetPositionInGrid().Y);
		asn->SetPassable(_canGoTroughWalls ? true : (t->IsPassable() ? true : false));
		asn->m_GCost = t->IsPassable() ? m_emptyTileGCost : m_DigCost;
		m_nodes.push_back(asn);

		asn = new AStarNode();
		asn->Init(i, t->GetPositionInGrid().X, t->GetPositionInGrid().Y);
		asn->SetPassable(_canGoTroughWalls ? true : (t->IsPassable() ? true : false));
		asn->m_GCost = t->IsPassable() ? m_emptyTileGCost : m_DigCost;
		m_modifableNodes.push_back(asn);
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
		asn->m_GCost = _world[i] > 0 ? m_DigCost : m_emptyTileGCost;
		m_nodes.push_back(asn);

		asn = new AStarNode();
		x = (i % _width);
		asn->Init(i, x, (i - x) / _width);
		asn->SetPassable(true);
		asn->m_GCost = _world[i] > 0 ? m_DigCost : 1;
		m_modifableNodes.push_back(asn);
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
void AStarSearch::cleanModifiableNodes() {

	for (size_t i = 0; i < m_modifableNodes.size(); i++)
	{
		if (m_modifableNodes[i] != nullptr)
		{
			delete(m_modifableNodes[i]);
			m_modifableNodes[i] = nullptr;
		}
	}
	m_modifableNodes.erase(m_modifableNodes.begin(), m_modifableNodes.end());
}

AStarNode* AStarSearch::getTileAtIndex(int _index, std::vector<AStarNode*>& _nodesToUse) {
	return _index > -1 && _index < _nodesToUse.size() ? _nodesToUse[_index] : nullptr;
}


