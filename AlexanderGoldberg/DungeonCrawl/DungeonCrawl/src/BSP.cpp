#include "BSP.h"
#include "BSPNode.h"
#include <string>
#include"World.h"
#include "Tile.h"
#include "AStarSearch.h"

BSP::BSP(int _gridWidth, int _gridHeight)
{
	m_width = _gridWidth;
	m_height = _gridHeight;
	
}

BSP::~BSP()
{
	WipeTree();
	
}


void BSP::BeginSplit(int _timesToSplit) {
	WipeTree();
	m_targetNumOfSplits = _timesToSplit;
	m_numberOfSplits = 0;
	m_tree.push_back(new BSPNode(0, 0, m_width, m_height, -1, 0));
	split();	
	m_previousRotations.erase(m_previousRotations.begin(), m_previousRotations.end());	
	if (!printedOnce)
	{
		
		printedOnce = true;
		printf("For a target number of splits %d the total size is %d and leaf count is %d\n", m_targetNumOfSplits, m_tree.size(), GetLeaves().size());
		printf("Total number of generation attempts is %d\n", m_generationAttempt);
	}
	
}

void BSP::SetTunnelingType(TunnelingType _tunnelingType)
{
	m_tunnelingType = _tunnelingType;
}

const std::string& BSP::GetEnumName(TunnelingType _enumValueToGetNameOf)
{
	return m_enumNames[(int)_enumValueToGetNameOf];
}

void BSP::split()
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
			RectA parent = m_tree[parentIndex -1]->GetRect();
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
			

			RectA rect1 = RectA();
			RectA rect2 = RectA();
			

			int mhps = m_ensureRoomSeperation ? 5 : 3;//Minimum Horizontal Partion Size
			int mvps = m_ensureRoomSeperation ? 5 : 3;//Minimum Vertical Partion Size
				

			if (direction == DTS::VERTICAL)
			{
				int width = parent.x2 - parent.x1;	
				int timesRound = 99;
				do
				{
					int splitPoint = (parent.x1 + (int)(width * 0.25f)) + rand() % (width - (int)(width * 0.25f) + 1);
					rect1.Set(parent.x1, parent.y1, splitPoint, parent.y2);
					rect2.Set(splitPoint, parent.y1, parent.x2, parent.y2);
					if (timesRound-- < 0)
					{
						success = false;
						printf("Unable to break out of do while loop for ensuring minimum room size. Splitting parent at index %d\n", parentIndex);
						break;
					}
				} while (!rect1.CheckIfMeetsOrExceedsMin(mhps, mvps) || !rect2.CheckIfMeetsOrExceedsMin(mhps, mvps));

			}
			if (direction == DTS::HORIZONTAL)
			{
				int height = parent.y2 - parent.y1;				
				int timesRound = 99;
				do
				{
					int splitPoint = (parent.y1 + (int)(height * 0.25f)) + std::rand() % (height - (int)(height * 0.25f) + 1);
					
					rect1.Set(parent.x1, parent.y1, parent.x2, splitPoint);
					rect2.Set(parent.x1, splitPoint, parent.x2, parent.y2);

					if (timesRound-- < 0)
					{
						success = false;
						printf("Unable to break out of do while loop for ensuring minimum room size. Splitting parent at index %d\n", parentIndex);
						break;
					}
				} while (!rect1.CheckIfMeetsOrExceedsMin(mhps, mvps) || !rect2.CheckIfMeetsOrExceedsMin(mhps, mvps));
			}
			if (success)
			{
				m_tree.push_back(new BSPNode(rect1, parentIndex - 1, m_tree.size() - 1));
				m_tree.push_back(new BSPNode(rect2, parentIndex - 1, m_tree.size() - 1));
			}
			
		}
		if (success)
		{
			split();
		}
		else
		{
			StartOver();
			return;
		}
	}

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

void BSP::StartOver() 
{
	WipeTree();
	m_generationAttempt++;
	BeginSplit(m_targetNumOfSplits);
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
	int y = (_index - x) / _width;

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
		m_IndexesOfStartEndPointsForPathSegments.erase(m_IndexesOfStartEndPointsForPathSegments.begin(), m_IndexesOfStartEndPointsForPathSegments.end());

		switch (m_tunnelingType)
		{
		case BSP::TunnelingType::FirstToLast:
			TunnelingWorkInwards(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::RoomToRoom:
			TunnelingRoomToRoom(_AStar, indexesOfRoomTiles, false, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::Hub:
			TunnelingSpiderOut(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration, true);
			break;
		case BSP::TunnelingType::Sequential:
			TunnelingSequential(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::RegionToRegion:
			TunnelingRegionToRegion(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		case BSP::TunnelingType::StringOfRooms:
			TunnelingCorridorsThroughRooms(_AStar, indexesOfRoomTiles, m_usePreviouslyDugPathsInPathGeneration);
			break;
		default:
			printf("No established function for case %d.\n", (int)m_tunnelingType);
			break;
		}
		
	}
	return m_usablePaths;
}

void BSP::TunnelingWorkInwards(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	int timesToDig = indexesOfRoomTiles.size() / 2;

	for (size_t i = 0; i < timesToDig; i++)
	{
		int lastRoom = indexesOfRoomTiles.size() - (i + 1);
		int firstRoom = i;
		int index1 = indexesOfRoomTiles[firstRoom][indexesOfRoomTiles[firstRoom].size() / 2];
		int index2 = indexesOfRoomTiles[lastRoom][indexesOfRoomTiles[lastRoom].size() / 2];

		int x1 = index1 % m_width;
		int y1 = ((index1 - x1) / m_width);
		int x2 = index2 % m_width;
		int y2 = ((index2 - x2) / m_width);
		//printf("\nAttempting dig from %d <%d, %d> to %d <%d, %d>.  This is dig %d\n", index1, x1, y1, index2, x2, y2, i);
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
		//printf("\nDug path from <%d, %d> to <%d, %d>.  This is dig %d\n", x1, y1, x2, y2, i);
	}
}
void BSP::TunnelingRoomToRoom(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _repeatRoomDigs, bool _updateMapWithPreviousPaths) {
	//path find from the top center of each room to the bottom center of each room
	// if _repeatRoomDigs there will be more paths connecting rooms and existing paths may be  wider
	const std::vector<RectA>& roomRegions = m_roomRegions;
	
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i++)
	{
		for (size_t j = (_repeatRoomDigs ? 0 : i); j < indexesOfRoomTiles.size(); j++)
		{
			if (i != j)// do not path find from a room to the same room
			{

				int startingTile = ((roomRegions[i].x2 - roomRegions[i].x1) * 0.5f);
				int endTile = indexesOfRoomTiles[j].size() - ((roomRegions[j].x2 - roomRegions[j].x1) * 0.5f);
				int index1 = indexesOfRoomTiles[i][startingTile];
				int index2 = indexesOfRoomTiles[j][endTile];
				std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
				int timesToPop = path.size();
				m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
				for (size_t j = 0; j < timesToPop; j++)
				{
					m_usablePaths.push_back(path.top());
					path.pop();
				}
				m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
			}
		}

	}
}
void BSP::TunnelingSpiderOut(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths, bool _randomizeWhichRoomIsOrigin, int _centralRoomToSpiralFrom) {
	//path find from the top center of each room to the bottom center of each room
	// if _repeatRoomDigs there will be more paths connecting rooms and existing paths may be  wider
	int originRoom = _centralRoomToSpiralFrom;
	if (_randomizeWhichRoomIsOrigin)
		originRoom = rand() % indexesOfRoomTiles.size();
	const std::vector<RectA>& roomRegions = m_roomRegions;
	int startingTile = ((roomRegions[originRoom].x2 - roomRegions[originRoom].x1) * 0.5f);
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i++)
	{
		//could add checks to see if endtile index is less than (above or to the left) or greater than (to the right or below)  this combined with some modulo action could set the end tile to be on the appropriate side
		if (i != originRoom)
		{
			int endTile = ((roomRegions[i].x2 - roomRegions[i].x1) * 0.5f); //top center of room
			int index1 = indexesOfRoomTiles[originRoom][startingTile];
			int index2 = indexesOfRoomTiles[i][endTile];
			std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
			int timesToPop = path.size();
			m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
			for (size_t j = 0; j < timesToPop; j++)
			{
				m_usablePaths.push_back(path.top());
				path.pop();
			}
			m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
		}
	}
}
void BSP::TunnelingSequential(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	

	for (size_t i = 0; i < indexesOfRoomTiles.size() - 1; i++)
	{
		int firstRoom = i;
		int secondRoom = i + 1;
		int index1 = indexesOfRoomTiles[firstRoom][indexesOfRoomTiles[firstRoom].size() / 2];
		int index2 = indexesOfRoomTiles[secondRoom][indexesOfRoomTiles[secondRoom].size() /2];

		
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
	}
}
void BSP::TunnelingRegionToRegion(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	//produces longer corridors
	std::vector<int> pathCenters;
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i+=2)
	{
		int index1 = indexesOfRoomTiles[i][0];
		int index2 = indexesOfRoomTiles[i + 1][0];

		
		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			if (j == timesToPop / 2)
			{
				pathCenters.push_back(path.top());
			}
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
	}
	for (size_t i = 0; i < pathCenters.size() / 2; i++)
	{
		int index1 = pathCenters[i];
		int index2 = pathCenters[(pathCenters.size() - 1) -  i];


		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
	}

}
void BSP::TunnelingCorridorsThroughRooms(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths) {
	//tends toward producing strings of rooms, occasionally with a central area, but rooms tend to run in strings with corridors running through them extending to corridor
	std::vector<int> pathCenters;
	for (size_t i = 0; i < indexesOfRoomTiles.size(); i += 2)
	{
		int index1 = indexesOfRoomTiles[i][0];
		int index2 = indexesOfRoomTiles[i + 1][0];


		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
	}

	for (size_t i = 1; i < indexesOfRoomTiles.size() - 1; i += 2)
	{
		int index1 = indexesOfRoomTiles[i][0];
		int index2 = indexesOfRoomTiles[i + 1][0];


		std::stack<int> path = _AStar.BeginSearch(index1, index2, _updateMapWithPreviousPaths);
		int timesToPop = path.size();
		m_IndexesOfStartEndPointsForPathSegments.push_back(Vector2(m_usablePaths.size(), 0));
		for (size_t j = 0; j < timesToPop; j++)
		{
			m_usablePaths.push_back(path.top());
			path.pop();
		}
		m_IndexesOfStartEndPointsForPathSegments[m_IndexesOfStartEndPointsForPathSegments.size() - 1].Y = m_usablePaths.size() - 1;
	}

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