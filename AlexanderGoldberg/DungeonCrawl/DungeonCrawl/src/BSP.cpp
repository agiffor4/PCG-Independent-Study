#include "BSP.h"
#include "BSPNode.h"
#include <string>
#include"World.h"
#include "Tile.h"
#include "AStarSearch.h"

#define PathStarts m_IndexesOfStartEndPointsForPathSegments
BSP::BSP(int _gridWidth, int _gridHeight)
{
	m_width = _gridWidth;
	m_height = _gridHeight;
	
}

BSP::~BSP()
{
	WipeTree();
	
}


bool BSP::BeginSplit(int _timesToSplit) {
	WipeTree();
	m_targetNumOfSplits = _timesToSplit;
	m_numberOfSplits = 0;
	m_tree.push_back(new BSPNode(0, 0, m_width, m_height, -1, 0));
	bool result = split();
	m_previousRotations.erase(m_previousRotations.begin(), m_previousRotations.end());	
	if (!printedOnce)
	{
		
		printedOnce = true;
		printf("For a target number of splits %d the total size is %d and leaf count is %d\n", m_targetNumOfSplits, m_tree.size(), GetLeaves().size());
		printf("Total number of generation attempts is %d\n", m_generationAttempt);
	}
	return result;
}


const std::string& BSP::GetEnumName(TunnelingType _enumValueToGetNameOf)
{
	return m_enumNames[(int)_enumValueToGetNameOf];
}

int BSP::GetRandomTileInRoom(int _roomRegion)
{
	std::vector<std::vector<int>> roomtiles = GetRoomTileIndexes();
	return roomtiles[_roomRegion][rand() % roomtiles[_roomRegion].size()];
}

bool BSP::split()
{
	
	m_numberOfSplits++;
	bool success = true;
	if (m_numberOfSplits < m_targetNumOfSplits)
	{
		size_t size = m_tree.size();
		for (size_t i = GetFirstLeafIndex(); i < size; i++)
		{
			DTS direction = ((DTS)(rand() % 2));
			bool isNextIndexEven = (m_tree.size() + 1) % 2 == 0;
			int nextSize = m_tree.size() + 1;
			int parentIndex = (isNextIndexEven) ? nextSize / 2 : (nextSize-1) / 2;
			m_parent = m_tree[parentIndex -1]->GetRect();
			if (m_previousRotations.size() > 2)
			{
				if (m_previousRotations[m_previousRotations.size() -1] == direction &&
					m_previousRotations[m_previousRotations.size() - 2] == direction)
				{
					while (direction == m_previousRotations[m_previousRotations.size() -1])
					{
						direction = ((DTS)(std::rand() % 2));
					}
				}
				
			}
			m_previousRotations.push_back(direction);
			

			int mhps = m_ensureRoomSeperation ? 5 : 3;//Minimum Horizontal Partion Size
			int mvps = m_ensureRoomSeperation ? 5 : 3;//Minimum Vertical Partion Size
				

			if (direction == DTS::VERTICAL)
			{
				int width = m_parent.x2 - m_parent.x1;
				int timesRound = 99;
				do
				{
					int splitPoint = (m_parent.x1 + (int)(width * 0.25f)) + rand() % (width - (int)(width * 0.25f) + 1);
					m_rect1.Set(m_parent.x1, m_parent.y1, splitPoint, m_parent.y2);
					m_rect2.Set(splitPoint, m_parent.y1, m_parent.x2, m_parent.y2);
					if (timesRound-- < 0)
					{
						success = false;
						printf("Unable to break out of do while loop for ensuring minimum room size. Splitting parent at index %d\n", parentIndex);
						break;
					}
				} while (!m_rect1.CheckIfMeetsOrExceedsMin(mhps, mvps) || !m_rect2.CheckIfMeetsOrExceedsMin(mhps, mvps));

			}
			if (direction == DTS::HORIZONTAL)
			{
				int height = m_parent.y2 - m_parent.y1;
				int timesRound = 99;
				do
				{
					int splitPoint = (m_parent.y1 + (int)(height * 0.25f)) + std::rand() % (height - (int)(height * 0.25f) + 1);
					
					m_rect1.Set(m_parent.x1, m_parent.y1, m_parent.x2, splitPoint);
					m_rect2.Set(m_parent.x1, splitPoint, m_parent.x2, m_parent.y2);

					if (timesRound-- < 0)
					{
						success = false;
						printf("Unable to break out of do while loop for ensuring minimum room size. Splitting parent at index %d\n", parentIndex);
						break;
					}
				} while (!m_rect1.CheckIfMeetsOrExceedsMin(mhps, mvps) || !m_rect2.CheckIfMeetsOrExceedsMin(mhps, mvps));
			}
			if (success)
			{
				m_tree.push_back(new BSPNode(m_rect1, parentIndex - 1, m_tree.size() - 1));
				m_tree.push_back(new BSPNode(m_rect2, parentIndex - 1, m_tree.size() - 1));
			}
			
		}
		if (success)
		{
			split();
		}
		else
		{
			return StartOver();
		}
	}
	return true;
}
void BSP::WipeTree() {
	for (size_t i = 0; i < m_tree.size(); i++)
	{
		if (m_tree[i] != nullptr)
		{
			delete(m_tree[i]);
			m_tree[i] = nullptr;
		}
	}
	m_tree.erase(m_tree.begin(), m_tree.end());
}

bool BSP::StartOver() 
{
	WipeTree();
	m_generationAttempt++;
	m_previousRotations.erase(m_previousRotations.begin(), m_previousRotations.end());
	printf("generation attempt %d\n", m_generationAttempt);
	if (m_generationAttempt > 90)
		return false;
	BeginSplit(m_targetNumOfSplits);
	return true;
}


void BSP::GenerateRoomsAndPaths(AStarSearch& _AStar, std::vector<std::vector<int>>& _generatedRooms, std::vector<int>& _generatedPaths)
{
	_generatedRooms = GetRoomTileIndexes();
	_generatedPaths = GeneratePaths(_AStar, true, &_generatedRooms);
}
std::vector<RectA> BSP::GetRoomRegions(bool _overwritePreviousRooms) {
	if (_overwritePreviousRooms || m_roomRegions.size() < 1)
	{
		std::vector<BSPNode*> leaves = GetLeaves();
		m_roomRegions.erase(m_roomRegions.begin(), m_roomRegions.end());

		for (size_t i = 0; i < leaves.size(); i++)
		{
			RectA currentPartion = leaves[i]->GetRect();
			RectA generatedRoom = RectA();
			int x1 = 0;
			int y1 = 0;
			int x2 = 0;
			int y2 = 0;
			int timesRound = 500;
			if (m_ensureRoomSeperation)
			{
				RectA partionWithEnforcedWalls = RectA(currentPartion.x1 + 1, currentPartion.y1 + 1, currentPartion.x2 - 1, currentPartion.y2 - 1);

				do
				{

					x1 = partionWithEnforcedWalls.x1 + rand() % (partionWithEnforcedWalls.x2 - partionWithEnforcedWalls.x1 + 1);
					y1 = partionWithEnforcedWalls.y1 + rand() % (partionWithEnforcedWalls.y2 - partionWithEnforcedWalls.y1 + 1);
					x2 = x1 + rand() % (partionWithEnforcedWalls.x2 - x1 + 1);
					y2 = y1 + rand() % (partionWithEnforcedWalls.y2 - y1 + 1);
					generatedRoom.Set(x1, y1, x2, y2);
				} while (generatedRoom.Area() < partionWithEnforcedWalls.Area() * 0.5f || !generatedRoom.CheckIfMeetsOrExceedsMin(3, 3));
				//room must use at least half the partion and be at least a 3x3 sized room

			}
			else
			{
				do
				{

					x1 = currentPartion.x1 + rand() % (currentPartion.x2 - currentPartion.x1 + 1);
					y1 = currentPartion.y1 + rand() % (currentPartion.y2 - currentPartion.y1 + 1);
					x2 = x1 + rand() % (currentPartion.x2 - x1 + 1);
					y2 = y1 + rand() % (currentPartion.y2 - y1 + 1);
					generatedRoom.Set(x1, y1, x2, y2);
				} while (generatedRoom.Area() < currentPartion.Area() * 0.5f || !generatedRoom.CheckIfMeetsOrExceedsMin(2, 2) ||
					generatedRoom.x1 == 0 ||
					generatedRoom.x2 == m_width ||
					generatedRoom.y1 == 0 ||
					generatedRoom.y2 == m_height
					);
				//room must use at least half the partion and be at least a 3x3 sized room
			}


			m_roomRegions.push_back(generatedRoom);
		}
	}
	return m_roomRegions;
}


int BSP::convertXYToIndex(int _x, int _y, int _width)
{
	return (_width * _y) + _x;
}

const Vector2 BSP::convertIndexToXY(int _index, int _width)
{
	int x = _index % _width;
	int y = (_index) / _width;

	return Vector2(x, y);
}


std::vector<std::vector<int>> BSP::GetPartions() {
	std::vector<std::vector<int>> partions = std::vector<std::vector<int>>();
	std::vector<BSPNode*> leaves = GetLeaves();
	for (size_t i = 0; i < leaves.size(); i++)
	{
		partions.push_back(std::vector<int>());
	}
	for (size_t i = 0; i < leaves.size(); i++)
	{
		RectA rect = leaves[i]->GetRect();
		int x1 = rect.x1;
		int y1 = m_height - rect.y1;
		int x2 = rect.x2;
		int y2 = m_height - rect.y2;
		for (size_t y = y2; y < y1; y++)
		{
			for (size_t x = x1; x < x2; x++)
			{
				partions[i].push_back((m_width * y) + x);
			}
		}
	}
	return partions;

}
std::vector<std::vector<int>> BSP::GetRoomTileIndexes() {
	std::vector<RectA> rooms = GetRoomRegions();
	//each std::vector<int> is a collection of indexes for a given room
	std::vector<std::vector<int>> roomTileIndexes = std::vector<std::vector<int>>();
	for (size_t i = 0; i < rooms.size(); i++)
	{
		roomTileIndexes.push_back(std::vector<int>());
		RectA rect = rooms[i];
		int x1 = rect.x1;
		int y1 = m_height - rect.y1;
		int x2 = rect.x2;
		int y2 = m_height - rect.y2;
		for (size_t y = y2; y < y1; y++)
		{
			for (size_t x = x1; x < x2; x++)
			{
				roomTileIndexes[i].push_back((m_width * y) + x);
			}
		}
	}
	return roomTileIndexes;
}



void BSP::printLeafResults() {
	std::vector<BSPNode*> leaves = GetLeaves();
	std::vector<int> mapToPrint = std::vector<int>();
	int size = m_height * m_width;
	//(m_width * y) + x
	for (size_t i = 0; i < size; i++)
	{
		mapToPrint.push_back(0);
	}

	std::vector<std::vector<int>> partions = GetPartions();
	for (size_t i = 0; i < partions.size(); i++)
	{
		for (size_t j = 0; j < partions[i].size(); j++)
		{
			mapToPrint[partions[i][j]] = (i + 7);
		}
	}
	std::vector<RectA> rooms = GetRoomRegions();
	
	std::vector<std::vector<int>> indexesOfRoomTiles = GetRoomTileIndexes();
	std::vector<int> paths;
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i++)
	{
		for (size_t j = 0; j < indexesOfRoomTiles[i].size(); j++)
		{
			mapToPrint[indexesOfRoomTiles[i][j]] = 0;
		}
	}	
	
	print(mapToPrint, m_width); //regions and rooms
	//begin digging
	AStarSearch AStar = AStarSearch();
	AStar.CastIntVectorToAStarNodes(mapToPrint, m_width);
	AStar.Initialize(Vector2(m_width, m_height), mapToPrint.size(), false);
	paths = GeneratePaths(AStar);
	for (size_t i = 0; i < paths.size(); i++)
		mapToPrint[paths[i]] = 0;

	//set the dig start point as -1 
	/*
	int timesToDig = indexesOfRoomTiles.size() / 2;
	for (size_t i = 0; i < timesToDig; i++)
	{
		int lastRoom = indexesOfRoomTiles.size() - (i + 1);
		int firstRoom = i;
		int index1 = indexesOfRoomTiles[firstRoom][indexesOfRoomTiles[firstRoom].size() / 2];
		int index2 = indexesOfRoomTiles[lastRoom][indexesOfRoomTiles[lastRoom].size() / 2];
		mapToPrint[index1] = -1;
		mapToPrint[index2] = -1;
		
	}*/
	print(mapToPrint, m_width); //halls
	
}
void BSP::print(std::vector<int>& _toPrint, int _width) {	
	printf("\n\n");
	int y = 24;
	std::string lineNumber = (y < 10 ? "0" : "") + std::to_string(y);
	printf("%s-", lineNumber.c_str());
	for (size_t i = 0; i < _toPrint.size(); i++)
	{
		std::string message = (_toPrint[i] < 10 ? "|0" : "|") + std::to_string(_toPrint[i]);
		if (_toPrint[i] == 0)
			message = "|__";
		if (_toPrint[i] == -1)
			message = "|AA";

		printf("%s", message.c_str());
		if (i % _width == _width - 1)
		{
			y--;
			if (y > -1)
			{
				std::string lineNumber = (y < 10 ? "0" : "") + std::to_string(y);
				printf("|\n%s-", lineNumber.c_str());
				
			}
			else
			{
				printf("|\nxx-");
			}
			
		}

	}
	for (size_t i = 0; i < 25; i++)
	{
		std::string lineNumber = (i < 10 ? "0" : "") + std::to_string(i);
		printf("|%s", lineNumber.c_str());
	}
	printf("|\n\n");
}

BSPNode* BSP::GetFirstLeaf() {
	if (m_tree.size() < 1)
	{
		return nullptr;
	}
	if (m_tree.size() % 2 == 0)
	{
		return m_tree[m_tree.size() - (m_tree.size() / 2)];
	}
	else
	{
		return m_tree[m_tree.size() - (((m_tree.size() - 1) / 2) + 1)];
	}
}
int BSP::GetFirstLeafIndex() {
	if (m_tree.size() < 1)
	{
		return 0;
	}
	if (m_tree.size() % 2 == 0)
	{
		return m_tree.size() - (m_tree.size() / 2);
	}
	else
	{
		return m_tree.size() - (((m_tree.size() - 1) / 2) + 1);
	}
}
std::vector<BSPNode*> BSP::GetLeaves()
{
	std::vector<BSPNode*> leaves;
	if (m_tree.size() < 1)
	{
		return leaves;
	}
	if (m_tree.size() % 2 == 0)
	{
		for (size_t i = m_tree.size() - (m_tree.size() / 2); i < m_tree.size(); i++)
		{
			leaves.push_back(m_tree[i]);
		}
	}
	else
	{
		for (size_t i = m_tree.size() - (((m_tree.size()-1) / 2) + 1); i < m_tree.size(); i++)
		{
			leaves.push_back(m_tree[i]);
		}
	}
	return leaves;
}

BSPNode* BSP::GetRandomLeaf()
{
	std::vector<BSPNode*> leaves = GetLeaves();
	return leaves[rand() % leaves.size()];
}

int BSP::Size()
{
	return m_tree.size();
}

std::vector<int> BSP::GeneratePaths(AStarSearch& _AStar, bool _overwritePreviousPaths, std::vector<std::vector<int>>* const _roomTileIndexes)
{


	if (_overwritePreviousPaths || m_usablePaths.size() < 1)
	{
		std::vector<std::vector<int>> indexesOfRoomTiles = _roomTileIndexes == nullptr ? GetRoomTileIndexes() : (*_roomTileIndexes);
		m_usablePaths.erase(m_usablePaths.begin(), m_usablePaths.end());
		PathStarts.erase(PathStarts.begin(), PathStarts.end());
		_AStar.SetWallDigCost(200);
		_AStar.SetEmptyTileCost(1);
		switch (m_tunnelingType)
		{
		case BSP::TunnelingType::Base:
			TunnelingBase(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::Hub:
			printf("hub center is %d\n", TunnelingHub(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration, false, true));
			break;
		case BSP::TunnelingType::StringOfRooms:
			TunnelingStringsOfRooms(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::RoomToRoom:			
			TunnelingRoomToRoom(_AStar, indexesOfRoomTiles, true, false); 
			break;  		
		case BSP::TunnelingType::RegionToRegion:
			TunnelingRegionToRegion(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		default:
			printf("No established function for case %d.\n", (int)m_tunnelingType);
			break;
		}
		
	}
	return m_usablePaths;
}

std::vector<int> BSP::GeneratePathsIncremental(AStarSearch& _AStar, int _index, bool& _result, bool _overwritePreviousPaths, std::vector<std::vector<int>>* const _roomTileIndexes)
{
		_result = false;
		std::vector<std::vector<int>> indexesOfRoomTiles = _roomTileIndexes == nullptr ? GetRoomTileIndexes() : (*_roomTileIndexes);
		_AStar.SetWallDigCost(200);
		_AStar.SetEmptyTileCost(1);
		switch (m_tunnelingType)
		{
		case BSP::TunnelingType::Base:
			_result = TunnelingBaseIncremental(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration, _index);
			break;
		case BSP::TunnelingType::Hub:
			TunnelingHubIncremental(_AStar, _index, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration, false, true);
			break;
		case BSP::TunnelingType::StringOfRooms:
			_result = TunnelingStringsOfRoomsIncremental(_AStar, _index, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::RoomToRoom:
			_result = TunnelingRoomToRoomIncremental(_AStar, _index, indexesOfRoomTiles);
			break;
		case BSP::TunnelingType::RegionToRegion:
			_result = TunnelingRegionToRegionIncremental(_AStar, _index, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		default:
			printf("No established function for case %d.\n", (int)m_tunnelingType);
			break;
		}

	
	return m_usablePaths;
}


int BSP::GenerateExitLocation(int _startingIndex, int _startingRegion, World& _world)
{
	Vector2 xyStart = convertIndexToXY(_startingIndex, m_width);
	
	int exitIndex = 0;
	//furthestExitAsTheCrowFlies(exitIndex, _startingRegion, xyStart);
	//spawnExitWithinRangeFromPlayer(exitIndex, _startingIndex, _world.GetMapDimentions().X * 0.25f, _world.GetMapDimentions().X * 0.5f);
	//spawnExitInRandomRoom(exitIndex, _startingRegion);
	longestPathToFromStart(exitIndex, _startingIndex, _world);
	switch (m_tunnelingType)
	{
	case BSP::TunnelingType::Base:		
		//furthestExitAsTheCrowFlies(exitIndex, _startingRegion, xyStart);
		break;
	case BSP::TunnelingType::RoomToRoom:
		break;
	case BSP::TunnelingType::Hub:
		break;
	case BSP::TunnelingType::StringOfRooms:		
		break;
	case BSP::TunnelingType::RegionToRegion:
		break;
	default:
		printf("Cannot cretate exit, unknown tunneling type enum with value %d.\n", (int)(m_tunnelingType));
		break;
	}

	return exitIndex;
}
void BSP::furthestExitAsTheCrowFlies(int& _exitIndex, int _startingRegion, Vector2& const _startingPosition) {
	std::vector<std::vector<int>> roomTileIndexes = GetRoomTileIndexes();
	float currentMaxDistance = 0;
	for (size_t i = 0; i < roomTileIndexes.size(); i++)
	{
		if (i != _startingRegion)
		{
			for (size_t j = 0; j < roomTileIndexes[i].size(); j++)
			{
				Vector2 otherXY = convertIndexToXY(roomTileIndexes[i][j], m_width);
				if (Vector2::GetDistanceGreaterThan(_startingPosition, otherXY, currentMaxDistance))
				{
					currentMaxDistance = Vector2::GetMagnitude(_startingPosition, otherXY);
					_exitIndex = roomTileIndexes[i][j];
				}
			}
		}
	}
}
void BSP::spawnExitInRandomRoom(int& _exitIndex, int _startingRegion)
{
	_exitIndex = -1;
	
	std::vector<std::vector<int>> rooms = GetRoomTileIndexes();
	int roomIndexToSpawnIn = rand() % rooms.size();
	while (roomIndexToSpawnIn == _startingRegion)
		roomIndexToSpawnIn = rand() % rooms.size();
	_exitIndex = GetRandomTileInRoom(roomIndexToSpawnIn);
}
void BSP::spawnExitWithinRangeFromPlayer(int& _exitIndex, int _startingIndex, int _minDist, int _maxDist) 
{
	std::vector<std::vector<int>> roomTileIndexes = GetRoomTileIndexes();
	std::vector<int> possibleTilesToSpawnIn;
	Vector2 startingPosition = convertIndexToXY(_startingIndex, m_width);
	for (size_t i = 0; i < roomTileIndexes.size(); i++)
	{
		for (size_t j = 0; j < roomTileIndexes[i].size(); j++)
		{
			Vector2 otherXY = convertIndexToXY(roomTileIndexes[i][j], m_width);
			int mag = Vector2::GetMagnitude(startingPosition, otherXY);
			if (mag >= _minDist && mag < _maxDist)
			{
				possibleTilesToSpawnIn.push_back(roomTileIndexes[i][j]);
			}
		}
		
	}
	_exitIndex = possibleTilesToSpawnIn[rand() % possibleTilesToSpawnIn.size()];
}
void BSP::longestPathToFromStart(int& _exitIndex, int _startingIndex, World& _world)
{
	AStarSearch AStar = AStarSearch();

	AStar.SetWallDigCost(1000);
	AStar.SetEmptyTileCost(1);
	AStar.Initialize(_world.GetMapDimentions(), _world.GetTileCount(), false);
	AStar.CastTilesToAStarNodes(_world, false);
	int currentTargetRoomRegion = -1;
	int currentPathLength = 0;	
	int targetIndex = -1;
	std::stack<int> pathToExit;
	
	std::vector<std::vector<int>> rooms = GetRoomTileIndexes();
	//int roomstartingIn = RoomIndexTileIsIn(_startingIndex);
	for (size_t j = 0; j < rooms.size(); j++)
	{
		targetIndex = rooms[j][rooms[j].size() / 2];
		
		std::stack<int> pathFromIToJ = AStar.BeginSearch(_startingIndex, targetIndex, false);
		if (pathFromIToJ.size() > currentPathLength)
		{
			currentPathLength = pathFromIToJ.size();
			pathToExit = pathFromIToJ;
			currentTargetRoomRegion = j;

		}
	}
	int finalPiece = 0;
	while (!pathToExit.empty())
	{
		finalPiece = pathToExit.top();
		/*if(_world.GetTileAtIndex(pathToExit.top())->IsPassable())
			_world.GetTileAtIndex(pathToExit.top())->changeImage("img/corridor_tile.bmp");
		else
			_world.GetTileAtIndex(pathToExit.top())->changeImage("img/blockC_tile.bmp");*/
		pathToExit.pop();
	}	
	_exitIndex = finalPiece;//GetRandomTileInRoom(currentTargetRoomRegion);
}

//PATH GENERATING FUNCTIONS
void BSP::SetTunnelingType(TunnelingType _tunnelingType)
{
	m_tunnelingType = _tunnelingType;
}
void BSP::SetIgnoreExistingPaths(bool _val)
{
	m_usePreviouslyDugPathsInPathGeneration = !_val;
}
bool BSP::GetIgnoreExistingPaths()
{
	return m_usePreviouslyDugPathsInPathGeneration;
}
void BSP::TunnelingBase(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	int timesToDig = indexesOfRoomTiles.size() / 2;
	_AStar.SetWallDigCost(m_width * m_height);
	for (size_t i = 0; i < timesToDig; i++)
	{
		int lastRoom = indexesOfRoomTiles.size() - (i + 1);
		int firstRoom = i;
		int x, y;
		int roomWidth = m_roomRegions[firstRoom].GetWidth();
		m_roomRegions[firstRoom].GetCenter(&x, &y);
		int index1 = indexesOfRoomTiles[firstRoom][y * roomWidth + x];

		m_roomRegions[lastRoom].GetCenter(&x, &y);
		roomWidth = m_roomRegions[lastRoom].GetWidth();
		int index2 = indexesOfRoomTiles[lastRoom][y * roomWidth + x];
		
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
		
	}
}
int BSP::TunnelingHub(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths, bool _randomizeWhichRoomIsOrigin, bool _tryToPickCenteralRoom, int _centralRoomToSpiralFrom) {
	
	// if _repeatRoomDigs there will be more paths connecting rooms and existing paths may be  wider
	int originRoom = _centralRoomToSpiralFrom;
	const std::vector<RectA>& roomRegions = m_roomRegions;
	_AStar.SetEmptyTileCost(5);
	_AStar.SetWallDigCost(0);
	Vector2 worldCenter = Vector2(m_tree[0]->GetRect().x2 / 2, m_tree[0]->GetRect().y2 / 2);
	if (_randomizeWhichRoomIsOrigin)
	{
		originRoom = rand() % indexesOfRoomTiles.size();
	}
	else
	{
		bool foundCenter = false;
		if (_tryToPickCenteralRoom)
		{
			for (size_t i = 0; i < roomRegions.size(); i++)
			{
				RectA r = roomRegions[i];
				if (r.Contains(worldCenter.X, worldCenter.Y))
				{
					originRoom = i;
					foundCenter = true;
					break;
				}
			}
			if (!foundCenter)
			{
				int currentMinDist = 9999;
				for (size_t i = 0; i < roomRegions.size(); i++)
				{
					RectA r = roomRegions[i];
					Vector2 RecCenter = Vector2();
					r.GetCenter(&RecCenter.X, &RecCenter.Y);
					if (Vector2::GetDistanceLessThan(RecCenter, worldCenter, currentMinDist))
					{
						currentMinDist = Vector2::GetMagnitude(RecCenter, worldCenter);
						originRoom = i;						
					}
				}
			}

		}
	}
	
	
	int x, y;
	int roomWidth = m_roomRegions[originRoom].GetWidth();
	m_roomRegions[originRoom].GetCenter(&x, &y);	
	int startingTile = (y * roomWidth + x);
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i++)
	{
		//could add checks to see if endtile index is less than (above or to the left) or greater than (to the right or below)  this combined with some modulo action could set the end tile to be on the appropriate side
		if (i != originRoom)
		{
			
			roomWidth = m_roomRegions[i].GetWidth();
			m_roomRegions[i].GetCenter(&x, &y);
			int index2 = indexesOfRoomTiles[i][y * roomWidth + x];
			int index1 = indexesOfRoomTiles[originRoom][startingTile];

			std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
			int timesToPop = path.size();
			PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
			for (size_t j = 0; j < timesToPop; j++)
			{
				m_usablePaths.push_back(path.top());
				path.pop();
			}
			PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
		}
	}
	return originRoom; //returns the room that is used as the origin
}
void BSP::TunnelingStringsOfRooms(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	
	//creates long strings of room with individual strings connected to eacother through certain rooms
	for (size_t i = 0; i < indexesOfRoomTiles.size() -1; i++)
	{
		int firstRoom = i;
		int secondRoom = i + 1;
		int x, y;
		int roomWidth = m_roomRegions[firstRoom].GetWidth();
		m_roomRegions[firstRoom].GetCenter(&x, &y);
		int index1 = indexesOfRoomTiles[firstRoom][y * roomWidth + x];

		m_roomRegions[secondRoom].GetCenter(&x, &y);
		roomWidth = m_roomRegions[secondRoom].GetWidth();
		int index2 = indexesOfRoomTiles[secondRoom][y * roomWidth + x];
		
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);		
		int timesToPop = path.size();
		PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
	}
}
void BSP::TunnelingRoomToRoom(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _repeatRoomDigs, bool _updateMapWithPreviousPaths) {
	//ensures all rooms are connected to all other rooms.  Almost all to all rooms will connect to all nearby rooms and thus have several exits.
	// if _repeatRoomDigs is false not every room will be connected
	//if _updateMapWithPreviousPaths is false generates more open areas

	const std::vector<RectA>& roomRegions = m_roomRegions;
	_AStar.SetWallDigCost(2);
	_AStar.SetEmptyTileCost(5);
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i++)
	{
		for (size_t j = (_repeatRoomDigs ? 0 : i); j < indexesOfRoomTiles.size(); j++)
		{
			if (i != j)// do not path find from a room to the same room
			{
				int x, y;
				int roomWidth = m_roomRegions[i].GetWidth();
				m_roomRegions[i].GetCenter(&x, &y);
				int startingTile = y * roomWidth + x;

				m_roomRegions[j].GetCenter(&x, &y);
				roomWidth = m_roomRegions[j].GetWidth();				
				int endTile = y * roomWidth + x;

				int index1 = indexesOfRoomTiles[i][startingTile];
				int index2 = indexesOfRoomTiles[j][endTile];
				std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
				int timesToPop = path.size();
				PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
				for (size_t j = 0; j < timesToPop; j++)
				{
					m_usablePaths.push_back(path.top());
					path.pop();
				}
				PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
			}
		}

	}
}


void BSP::TunnelingRegionToRegion(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	//tends to results in groupsregions of rooms each roughly containing a third of the rooms, with one region connecting to given regions
	//favors digging through walls to connect the regions
	_AStar.SetEmptyTileCost(200);
	_AStar.SetWallDigCost(1);
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i+=2)
	{
		int index1 = indexesOfRoomTiles[i][0];
		int index2 = indexesOfRoomTiles[i + 1][0];
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
		
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
	}
	

	for (size_t i = 0; i < indexesOfRoomTiles.size()-2; i += 2)
	{
		int midpoint1 = indexesOfRoomTiles[i].size() / 2;
		int midpoint2 = indexesOfRoomTiles[i+2].size() / 2;
		int index1 = indexesOfRoomTiles[i][midpoint1];
		int index2 = indexesOfRoomTiles[i +2][midpoint2];
		printf("Connecting %d to %d\n", index1, index2);		
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
	}
}


bool BSP::TunnelingBaseIncremental(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths, int _index) {
	int timesToDig = indexesOfRoomTiles.size() / 2;
	int i = _index;
	_AStar.SetWallDigCost(m_width * m_height);
	if (i < timesToDig)
	{
		int lastRoom = indexesOfRoomTiles.size() - (i + 1);
		int firstRoom = i;
		int x, y;
		int roomWidth = m_roomRegions[firstRoom].GetWidth();
		m_roomRegions[firstRoom].GetCenter(&x, &y);				
		int index1 = indexesOfRoomTiles[firstRoom][y * roomWidth + x];
		m_roomRegions[lastRoom].GetCenter(&x, &y);
		roomWidth = m_roomRegions[lastRoom].GetWidth();
		int index2 = indexesOfRoomTiles[lastRoom][y * roomWidth + x];		

		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
		return false;
	}
	else
	{
		printf("Final path created\n");
		m_usablePaths.erase(m_usablePaths.begin(), m_usablePaths.end());
		PathStarts.erase(PathStarts.begin(), PathStarts.end());
		return true;
	}
}
int BSP::TunnelingHubIncremental(AStarSearch& _AStar, int _index, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths, bool _randomizeWhichRoomIsOrigin, bool _tryToPickCenteralRoom, int _centralRoomToSpiralFrom) {

	// if _repeatRoomDigs there will be more paths connecting rooms and existing paths may be  wider
	int originRoom = _centralRoomToSpiralFrom;
	const std::vector<RectA>& roomRegions = m_roomRegions;

	Vector2 worldCenter = Vector2(m_tree[0]->GetRect().x2 / 2, m_tree[0]->GetRect().y2 / 2);
	if (_randomizeWhichRoomIsOrigin)
	{
		originRoom = rand() % indexesOfRoomTiles.size();
	}
	else
	{
		bool foundCenter = false;
		if (_tryToPickCenteralRoom)
		{
			for (size_t i = 0; i < roomRegions.size(); i++)
			{
				RectA r = roomRegions[i];
				if (r.Contains(worldCenter.X, worldCenter.Y))
				{
					originRoom = i;
					foundCenter = true;
					break;
				}
			}
			if (!foundCenter)
			{
				int currentMinDist = 9999;
				for (size_t i = 0; i < roomRegions.size(); i++)
				{
					RectA r = roomRegions[i];
					Vector2 RecCenter = Vector2();
					r.GetCenter(&RecCenter.X, &RecCenter.Y);
					if (Vector2::GetDistanceLessThan(RecCenter, worldCenter, currentMinDist))
					{
						currentMinDist = Vector2::GetMagnitude(RecCenter, worldCenter);
						originRoom = i;
					}
				}
			}

		}
	}


	int x, y;
	int roomWidth = m_roomRegions[originRoom].GetWidth();
	m_roomRegions[originRoom].GetCenter(&x, &y);
	int startingTile = (y * roomWidth + x);
	int i = _index;
	if(i < indexesOfRoomTiles.size())
	{
		//could add checks to see if endtile index is less than (above or to the left) or greater than (to the right or below)  this combined with some modulo action could set the end tile to be on the appropriate side
		if (i != originRoom)
		{

			roomWidth = m_roomRegions[i].GetWidth();
			m_roomRegions[i].GetCenter(&x, &y);
			int index2 = indexesOfRoomTiles[i][y * roomWidth + x];
			int index1 = indexesOfRoomTiles[originRoom][startingTile];

			std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
			int timesToPop = path.size();
			PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
			for (size_t j = 0; j < timesToPop; j++)
			{
				m_usablePaths.push_back(path.top());
				path.pop();
			}
			PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
		}
	}
	return originRoom; //returns the room that is used as the origin
}
bool BSP::TunnelingStringsOfRoomsIncremental(AStarSearch& _AStar, int _index, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {

	//creates long strings of room with individual strings connected to eacother through certain rooms
	int i = _index;
	if(i < indexesOfRoomTiles.size() - 1)
	{


		int firstRoom = i;
		int secondRoom = i + 1;
		int x, y;
		int roomWidth = m_roomRegions[firstRoom].GetWidth();
		m_roomRegions[firstRoom].GetCenter(&x, &y);
		int index1 = indexesOfRoomTiles[firstRoom][y * roomWidth + x];

		m_roomRegions[secondRoom].GetCenter(&x, &y);
		roomWidth = m_roomRegions[secondRoom].GetWidth();
		int index2 = indexesOfRoomTiles[secondRoom][y * roomWidth + x];

		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
		return false;
	}
	return true;

}
bool BSP::TunnelingRoomToRoomIncremental(AStarSearch& _AStar, int _index, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _repeatRoomDigs, bool _updateMapWithPreviousPaths) {
	//ensures all rooms are connected to all other rooms.  Almost all to all rooms will connect to all nearby rooms and thus have several exits.
	// if _repeatRoomDigs is false not every room will be connected
	//if _updateMapWithPreviousPaths is false generates more open areas

	const std::vector<RectA>& roomRegions = m_roomRegions;
	_AStar.SetWallDigCost(2);
	_AStar.SetEmptyTileCost(5);
	int i = _index;
	if(i < indexesOfRoomTiles.size())
	{
		for (size_t j = (_repeatRoomDigs ? 0 : i); j < indexesOfRoomTiles.size(); j++)
		{
			if (i != j)// do not path find from a room to the same room
			{

				int x, y;
				int roomWidth = m_roomRegions[i].GetWidth();
				m_roomRegions[i].GetCenter(&x, &y);
				int startingTile = y * roomWidth + x;

				m_roomRegions[j].GetCenter(&x, &y);
				roomWidth = m_roomRegions[j].GetWidth();
				int endTile = y * roomWidth + x;

				int index1 = indexesOfRoomTiles[i][startingTile];
				int index2 = indexesOfRoomTiles[j][endTile];
				std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
				int timesToPop = path.size();
				PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
				for (size_t j = 0; j < timesToPop; j++)
				{
					m_usablePaths.push_back(path.top());
					path.pop();
				}
				PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
			}
		}
		return false;
	}
	return true;
}


bool BSP::TunnelingRegionToRegionIncremental(AStarSearch& _AStar, int _index, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	//tends to results in groupsregions of rooms each roughly containing a third of the rooms, with one region connecting to given regions
	//favors digging through walls to connect the regions
	_AStar.SetEmptyTileCost(200);
	_AStar.SetWallDigCost(1);
	int i = _index;
	if (i < indexesOfRoomTiles.size())
	{
		if (i < indexesOfRoomTiles.size())
		{
			int index1 = indexesOfRoomTiles[i][0];
			int index2 = indexesOfRoomTiles[i + 1][0];
			std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
			int timesToPop = path.size();
			PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
			for (size_t j = 0; j < timesToPop; j++)
			{

				m_usablePaths.push_back(path.top());
				path.pop();
			}
			PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;


		}


		for (size_t i = 0; i < indexesOfRoomTiles.size() - 2; i += 2)
		{
			int midpoint1 = indexesOfRoomTiles[i].size() / 2;
			int midpoint2 = indexesOfRoomTiles[i + 2].size() / 2;
			int index1 = indexesOfRoomTiles[i][midpoint1];
			int index2 = indexesOfRoomTiles[i + 2][midpoint2];
			printf("Connecting %d to %d\n", index1, index2);
			std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
			int timesToPop = path.size();
			PathStarts.push_back(Vector2(m_usablePaths.size(), 0));
			for (size_t j = 0; j < timesToPop; j++)
			{
				m_usablePaths.push_back(path.top());
				path.pop();
			}
			PathStarts[PathStarts.size() - 1].Y = m_usablePaths.size() - 1;
		}
		return false;
	}
	return true;

}






std::vector<std::vector<int>> BSP::GetCorridorOnlyTiles() {
	std::vector<std::vector<int>> corridorTiles;//generates vector of tiles that form corridors, but are not in rooms
	for (size_t i = 0; i < PathStarts.size(); i++)
	{
		corridorTiles.push_back(std::vector<int>());
		for (size_t j = PathStarts[i].X; j < PathStarts[i].Y; j++)
		{
			if (RoomIndexTileIsIn(m_usablePaths[j]) == -1)
			{
				corridorTiles[i].push_back(m_usablePaths[j]);
			}
		}
	}
	return corridorTiles;
}

std::set<int> BSP::GetRoomsCorridorConnectsTo(int _tileInCorridorSegment, World& _world)
{
	std::vector<std::vector<int>> rooms = GetRoomTileIndexes();
	std::set<int> uniqueRooms;
	std::set<int> openList;
	std::set<int> closedList;
	openList.emplace(_tileInCorridorSegment);
	while (!openList.empty())
	{
		int currentIndex = (*openList.begin());
		closedList.emplace(currentIndex);		
		openList.erase(openList.begin());
		std::vector<Tile*> neighbors  = _world.GetNeighbors(currentIndex, false);
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Tile* t = neighbors[i];
			bool inClosedList = closedList.find(t->GetPositionInVector()) != closedList.end();
			if (t->IsPassable(true) && !inClosedList)
			{
				if (t->IsCorridor())
				{
					openList.emplace(t->GetPositionInVector());
				}
				else
				{
					uniqueRooms.emplace(RoomIndexTileIsIn(t->GetPositionInVector(), &rooms));
				}
			}
		}
	}
	
	
	return uniqueRooms;

}
void BSP::ExitsFromRoom(int _roomIndex, int& _totalExits, std::set<int>& _connectedRooms, std::set<int>& _corridorIndexes, World& _world)
{
	std::vector<std::vector<int>> roomIndexes = GetRoomTileIndexes();
	std::set<int> corridorPoints;
	_totalExits = 0;
	int roomWidth = m_roomRegions[_roomIndex].GetWidth();
	int brTile = roomIndexes[_roomIndex][roomIndexes[_roomIndex].size() - 1]; //bottom right tile
	int tlTile = roomIndexes[_roomIndex][0]; //top left tile
	//checking top row
	int startIndex = tlTile;
	int endIndex = tlTile + roomWidth;

	Tile* tile = nullptr;
	//check Top row
	for (size_t j = startIndex; j < endIndex; j++)
	{
		tile = _world.GetAdjacentTile(j, World::TileDirection::UP);
		if (tile->IsCorridor())
		{
			corridorPoints.emplace(tile->GetPositionInVector());
		}
	}
	
	//checking bottom row
	startIndex = brTile - roomWidth + 1;
	endIndex = brTile + 1;
	for (size_t j = startIndex; j < endIndex; j++)
	{
		tile = _world.GetAdjacentTile(j, World::TileDirection::DOWN);
		if (tile->IsCorridor())
		{
			corridorPoints.emplace(tile->GetPositionInVector());
		}
	}

	//checking right column
	startIndex = tlTile + roomWidth -1;
	endIndex = brTile + m_width;
	for (size_t j = startIndex; j < endIndex; j+= m_width)
	{
		tile = _world.GetAdjacentTile(j, World::TileDirection::RIGHT);
		if (tile->IsCorridor())
		{
			corridorPoints.emplace(tile->GetPositionInVector());
		}
	}

	startIndex = tlTile;
	endIndex = brTile - roomWidth + m_width + 1;
	for (size_t j = startIndex; j < endIndex; j += m_width)
	{
		tile = _world.GetAdjacentTile(j, World::TileDirection::LEFT);
		if (tile->IsCorridor())
		{
			corridorPoints.emplace(tile->GetPositionInVector());
		}
	}

	std::set<int> connections;
	for (auto i = corridorPoints.cbegin(); i != corridorPoints.cend(); i++)
	{		
		std::set<int> con = GetRoomsCorridorConnectsTo((*i), _world);
		for (auto j = con.cbegin(); j != con.cend(); j++)
			connections.emplace((*j));
	}
	_corridorIndexes = corridorPoints;
	_connectedRooms = connections;
	_totalExits = corridorPoints.size();
}

std::vector<Vector2> BSP::GetPathStartAndEndIndexs()
{
	std::vector<Vector2> starEndIndexs;
	for (size_t i = 0; i < PathStarts.size(); i++)
	{
		starEndIndexs.push_back(Vector2());
		starEndIndexs[i].X = m_usablePaths[PathStarts[i].X];
		starEndIndexs[i].Y = m_usablePaths[PathStarts[i].Y];
	}
	return starEndIndexs;
}

void BSP::GetDoorPlacement(std::vector<int>& _doorLocation, std::vector<RoomData>& _roomData, int _playerStart, int _roomToGenerateDoorsIn)
{
	std::vector<std::vector<int>> rooms = GetRoomTileIndexes();	
	auto itt = _roomData[_roomToGenerateDoorsIn].sm_CorridorExits.cbegin();
	auto ittEnd = _roomData[_roomToGenerateDoorsIn].sm_CorridorExits.cend();
	while (itt != ittEnd)
	{
		_doorLocation.push_back((*itt));
		itt++;
	}
	
	

}

int BSP::RoomIndexTileIsIn(int _tileIndex, const std::vector<std::vector<int>>* _roomTileIndexesToUse) {
	//returns -1 if it cannot find an acceptable room
	std::vector<std::vector<int>> rooms;
	if (_roomTileIndexesToUse == nullptr)
		rooms = GetRoomTileIndexes();
	else
		rooms = (*_roomTileIndexesToUse);

	
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (IsIndexInRoom(_tileIndex, i))
		{
			return i;
		}

	}
	return -1;

}
bool BSP::IsIndexInRoom(int _tileIndex, int _roomIndex) {
	
	std::vector<std::vector<int>> rooms = GetRoomTileIndexes();
	Vector2 v = convertIndexToXY(_tileIndex, m_width);
	Vector2 topLeft = convertIndexToXY(rooms[_roomIndex][0], m_width);
	Vector2 bottomRight = convertIndexToXY(rooms[_roomIndex][rooms[_roomIndex].size()-1], m_width);
	bool result = v.X >= topLeft.X&&
		v.X <= bottomRight.X &&
		v.Y >= topLeft.Y&&
		v.Y <= bottomRight.Y;
	return result;
		
	
	/*
	std::vector<std::vector<int>> rooms = GetRoomTileIndexes();
	if (_tileIndex < rooms[_roomIndex][0] || _tileIndex > rooms[_roomIndex][rooms[_roomIndex].size() - 1])
		return false;
	for (size_t j = 0; j < rooms[_roomIndex].size(); j++)
	{
		if (rooms[_roomIndex][j] == _tileIndex)
		{
			return true;
		}
	}
	return false;
	*/
	
}

/*
void BSP::Tunneling4(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {


	for (size_t i = 0; i < indexesOfRoomTiles.size() - 1; i++)
	{
		int index1 = indexesOfRoomTiles[i][0];
		int index2 = indexesOfRoomTiles[i+1][0];


		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}

	}
}
*/