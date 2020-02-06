#include "BSP.h"
#include "BSPNode.h"
#include <string>
#include"World.h"
#include "Tile.h"
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
	
	m_targetNumOfSplits = _timesToSplit;
	m_numberOfSplits = 0;
	m_tree.push_back(new BSPNode(0, 0, m_width, m_height, -1, 0));
	split();
	printf("For a target number of splits %d the total size is %d and leaf count is %d\n", m_targetNumOfSplits, m_tree.size(), GetLeaves().size());
	m_previousRotations.erase(m_previousRotations.begin(), m_previousRotations.end());		
	printLeafResults();
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
				} while (!rect1.CheckIfMeetsOrExceedsMin(3, 3) || !rect2.CheckIfMeetsOrExceedsMin(3, 3));

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
				} while (!rect1.CheckIfMeetsOrExceedsMin(3, 3) || !rect2.CheckIfMeetsOrExceedsMin(3, 3));
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
	BeginSplit(m_targetNumOfSplits);
}

std::vector<RectA> BSP::GenerateRooms() {
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
		float a1 = 0;
		float a2 = currentPartion.Area() * 0.5f;
		bool minSize = false;
		do
		{
			
			x1 = currentPartion.x1 + rand() % (currentPartion.x2 - currentPartion.x1 + 1);
			y1 = currentPartion.y1 + rand() % (currentPartion.y2 - currentPartion.y1 + 1);
			x2 = x1 + rand() % (currentPartion.x2 - x1 + 1);
			y2 = y1 + rand() % (currentPartion.y2 - y1 + 1);;
			generatedRoom.Set(x1, y1, x2, y2);
			a1 = generatedRoom.Area();
			minSize = generatedRoom.CheckIfMeetsOrExceedsMin(3, 3);
		} while (a1 <  a2 && !minSize); 
		//room must use at least half the partion and be at least a 3x3 sized room
		roomRegions.push_back(generatedRoom);
		
	}
	return roomRegions;
}

void BSP::print(std::vector<int>& _toPrint, int _width) {
	//print
	printf("\n\n");
	for (size_t i = 0; i < _toPrint.size(); i++)
	{
		std::string message = _toPrint[i] < 10 ? "|0" + std::to_string(_toPrint[i]) : "|" + std::to_string(_toPrint[i]);
		
		printf("%s", message.c_str());
		if (i % _width == _width - 1)
		{
			printf("\n");
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

	std::vector<RectA> rooms = GenerateRooms();
	

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
