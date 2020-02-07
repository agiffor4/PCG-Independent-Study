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
		printLeafResults();
		printf("For a target number of splits %d the total size is %d and leaf count is %d\n", m_targetNumOfSplits, m_tree.size(), GetLeaves().size());
		printf("Total number of generation attempts is %d\n", m_generationAttempt);
	}
	
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

std::vector<RectA> BSP::generateRooms() {
	std::vector<BSPNode*> leaves = GetLeaves();
	std::vector<RectA> roomRegions;

	for (size_t i = 0; i < leaves.size(); i++)
	{
		RectA currentPartion = leaves[i]->GetRect();
		RectA generatedRoom = RectA();
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		int timesRound = 500;
		if (leaves[i]->GetSelfIndex() == 9)
		{
			int foo = 0;
		}
		if (m_ensureRoomSeperation)
		{
			RectA partionWithEnforcedWalls = RectA(currentPartion.x1 + 1, currentPartion.y1 + 1, currentPartion.x2 - 1, currentPartion.y2 -1);
			
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
			} while (generatedRoom.Area() < currentPartion.Area() * 0.5f || !generatedRoom.CheckIfMeetsOrExceedsMin(3, 3) ||
				generatedRoom.x1 == 0 ||
				generatedRoom.x2 == m_width ||
				generatedRoom.y1 == 0 ||
				generatedRoom.y2 == m_height
				);
			//room must use at least half the partion and be at least a 3x3 sized room
		}
		
		
		roomRegions.push_back(generatedRoom);
		
	}
	return roomRegions;
}

std::vector<int> BSP::generatePaths()
{
	return std::vector<int>();
}

void BSP::print(std::vector<int>& _toPrint, int _width) {
	//print
	printf("\n\n");
	for (size_t i = 0; i < _toPrint.size(); i++)
	{
		std::string message = _toPrint[i] < 10 ? "|0" + std::to_string(_toPrint[i]) : "|" + std::to_string(_toPrint[i]);
		if (_toPrint[i] == 0)
		{
			message = "|__";
		}
		printf("%s", message.c_str());
		if (i % _width == _width - 1)
		{
			printf("|\n");
		}

	}
	printf("\n\n");
}
std::vector<std::vector<int>> BSP::GetPartions(World* _world) {
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
				partions[i].push_back(_world->GetTileAtPosition(x, y)->GetPositionInVector());
			}
		}
	}
	return partions;

}
std::vector<std::vector<int>> BSP::GetRooms(World* _world)
{
	//each std::vector<int> is a collection of indexes for a given room
	std::vector<std::vector<int>> indexesOfRoomTiles = std::vector<std::vector<int>>();
	std::vector<RectA> rooms = generateRooms();
	for (size_t i = 0; i < rooms.size(); i++)
	{
		indexesOfRoomTiles.push_back(std::vector<int>());
		RectA rect = rooms[i];
		int x1 = rect.x1;
		int y1 = m_height - rect.y1;
		int x2 = rect.x2;
		int y2 = m_height - rect.y2;
		for (size_t y = y2; y < y1; y++)
		{
			for (size_t x = x1; x < x2; x++)
			{
				indexesOfRoomTiles[i].push_back(_world->GetTileAtPosition(x, y)->GetPositionInVector());
			}
		}
	}
	return indexesOfRoomTiles;

}

void BSP::printLeafResults() {
	std::vector<BSPNode*> leaves = GetLeaves();
	std::vector<int> mapToPrint = std::vector<int>();
	//(m_width * y) + x
	for (size_t i = 0; i < m_height * m_width; i++)
	{
		mapToPrint.push_back(0);
	}
	
	print(mapToPrint, m_width);

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
				mapToPrint[(m_width * y) + x] = (leaves[i]->GetSelfIndex() + 1);
			}
		}
	}

	print(mapToPrint, m_width);

	std::vector<RectA> rooms = generateRooms();
	

	for (size_t i = 0; i < rooms.size(); i++)
	{
		RectA rect = rooms[i];
		int x1 = rect.x1;
		int y1 = m_height - rect.y1;
		int x2 = rect.x2;
		int y2 = m_height - rect.y2;
		for (size_t y = y2; y < y1; y++)
		{
			for (size_t x = x1; x < x2; x++)
			{
				mapToPrint[(m_width * y) + x] = 0;
			}
		}
	}

	print(mapToPrint, m_width);


	AStarSearch AStar = AStarSearch();
	AStar.CastIntVectorToAStarNodes(mapToPrint, m_width);
	AStar.Initialize(Vector2(m_width, m_height), mapToPrint.size());
	int index2 = mapToPrint.size() - (m_width * 3) + 12;
	int index1 = mapToPrint.size() - (m_width *2) +2;
	AStar.BeginSearch(index1, index2);
	

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
