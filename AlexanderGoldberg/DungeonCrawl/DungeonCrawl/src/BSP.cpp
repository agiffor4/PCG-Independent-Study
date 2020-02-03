#include "BSP.h"
#include "BSPNode.h"
#include <string>
BSP::BSP(int _gridWidth, int _gridHeight)
{
	m_width = _gridWidth;
	m_height = _gridHeight;
	
}

BSP::~BSP()
{

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


void BSP::BeginSplit(int _timesToSplit) {
	
	m_targetNumOfSplits = _timesToSplit;
	m_tree.push_back(new BSPNode(0, 0, m_width, m_height, -1, 0));
	split();
	printf("For a target number of splits %d the total size is %d and leaf count is %d\n", m_targetNumOfSplits, m_tree.size(), GetLeaves().size());
	m_previousRotations.erase(m_previousRotations.begin(), m_previousRotations.end());
	printLeafResults();
}

void BSP::split()
{
	
	m_numberOfSplits++;
	if (m_numberOfSplits < m_targetNumOfSplits)
	{
		size_t size = m_tree.size();
		for (size_t i = GetFirstLeafIndex(); i < size; i++)
		{
			DTS direction = ((DTS)(rand() % 2));
			bool isNextIndexEven = (m_tree.size() + 1) % 2 == 0;
			int nextSize = m_tree.size() + 1;
			int parentIndex = (isNextIndexEven) ? nextSize / 2 : (nextSize-1) / 2;
			BSPNode::RectA parent = m_tree[parentIndex -1]->GetRect();
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
			if (direction == DTS::VERTICAL)
			{
				int width = parent.x2 - parent.x1;
				int splitPoint = (parent.x1 + (int)(width * 0.25f)) + rand() % (width - (int)(width * 0.25f) + 1);
				m_tree.push_back(new BSPNode(parent.x1, parent.y1, splitPoint, parent.y2, parentIndex - 1, m_tree.size() -1));
				m_tree.push_back(new BSPNode(splitPoint, parent.y1, parent.x2, parent.y2, parentIndex - 1, m_tree.size() -1));



			}
			if (direction == DTS::HORIZONTAL)
			{
				int width = parent.y2 - parent.y1;
				int splitPoint = (parent.y1 + (int)(width * 0.25f))+ std::rand() % (width - (int)(width * 0.25f) + 1);
				m_tree.push_back(new BSPNode(parent.x1, parent.y1, parent.x2, splitPoint, parentIndex - 1, m_tree.size() -1));
				m_tree.push_back(new BSPNode(parent.x1, splitPoint, parent.x2, parent.y2, parentIndex - 1, m_tree.size() -1));

			}
		}
		split();
	}

}

void BSP::printLeafResults() {
	int sizeOfTree = m_tree.size();
	std::vector<BSPNode*> leaves = GetLeaves();
	std::vector<int> mapToPrint = std::vector<int>();
	//(m_width * y) + x
	for (size_t i = 0; i < m_height * m_width; i++)
	{
		mapToPrint.push_back(-1);
	}
	
	for (size_t i = 0; i < leaves.size(); i++)
	{
		BSPNode::RectA rect = leaves[i]->GetRect();
		int x1 = rect.x1;
		int y1 = 32 - rect.y1;
		int x2 = rect.x2;
		int y2 = 32 - rect.y2;
		for (size_t y = y2; y < y1; y++)
		{
			for (size_t x = x1; x < x2; x++)
			{
				mapToPrint[(m_width * y) + x] = leaves[i]->GetSelfIndex() + 1;
			}
		}
		

	}


	//print
	printf("\n\n");
	for (size_t i = 0; i < mapToPrint.size(); i++)
	{
		printf("%s", std::to_string(mapToPrint[i]).c_str());
		if (i % m_width == m_width -1)
		{
			printf("\n");
		}
		
	}
	printf("\n\n");
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
