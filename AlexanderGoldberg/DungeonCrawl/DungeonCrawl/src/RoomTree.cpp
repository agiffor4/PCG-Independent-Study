#include "RoomTree.h"
#include "RoomTreeNode.h"
#include <set>

RoomTree::RoomTree()
{
	
		
}
RoomTree::~RoomTree()
{
	for (size_t i = 0; i < m_nodesInTree.size(); i++)
		delete(m_nodesInTree[i]);
	
}

void RoomTree::GenerateRoomTree(const std::vector<RoomData>& _roomData, int _playerStartRoom)
{	
	for (size_t i = 0; i < _roomData.size(); i++)
	{
		m_nodesInTree.push_back(new RoomTreeNode(nullptr));
		m_nodesInTree[i]->m_Index = i;
	}
	generateConnections(m_nodesInTree[_playerStartRoom], _roomData);

}
/*
bool RoomTree::hasBeenChecked() {

}*/
void RoomTree::generateConnections(RoomTreeNode* _parent, const std::vector<RoomData>& _roomData)
{
	
	int currentIndex = _parent->m_Index;
	int currentParentIndex = -1;

	if (_parent->GetParent() != nullptr)
	{
		currentParentIndex = _parent->GetParent()->m_Index;
	}
	m_nodesInTree[currentIndex]->m_Checked = true;
	if (_roomData[currentIndex].sm_exitCount > 1)
	{
		auto itt = _roomData[currentIndex].sm_regionsExitingTo.cbegin();
		while (itt != _roomData[currentIndex].sm_regionsExitingTo.cend()) //for each room connected to
		{
			int roomIndex = (*itt);
			bool notBackTracking = currentParentIndex != -1 ? roomIndex != _roomData[currentParentIndex].sm_region : true;
			if (roomIndex != _roomData[currentIndex].sm_region && notBackTracking)//ignore connections to self
			{
				if (!m_nodesInTree[roomIndex]->m_Checked)
				{
					m_nodesInTree[currentIndex]->AddChild(m_nodesInTree[roomIndex]);
					m_nodesInTree[roomIndex]->SetParent(m_nodesInTree[currentIndex]);
					generateConnections(m_nodesInTree[roomIndex], _roomData);
				}
				else
				{
					m_nodesInTree[roomIndex]->m_ChildAndParent = true;
				}
			}
			itt++;
		}
	}
}