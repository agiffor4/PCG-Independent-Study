#include "RoomTree.h"
#include "RoomTreeNode.h"
#include <set>

RoomTree::RoomTree()
{
	
		
}
RoomTree::~RoomTree()
{
	Clear();
}


void RoomTree::Clear() {
	for (size_t i = 0; i < m_nodesInTree.size(); i++)
		delete(m_nodesInTree[i]);
	m_nodesInTree.clear();
}
void RoomTree::GenerateRoomTree(const std::vector<RoomData>& _roomData, int _playerStartRoom)
{
	Clear();
	for (size_t i = 0; i < _roomData.size(); i++)
	{
		m_nodesInTree.push_back(new RoomTreeNode(nullptr));
		m_nodesInTree[i]->m_Index = i;
	}	
	generateConnections(m_nodesInTree[_playerStartRoom], _roomData);
	m_deepestDepth = -1;
	for (size_t i = 0; i < m_nodesInTree.size(); i++)
	{
		if (m_nodesInTree[i]->GetDepth() > m_deepestDepth)
			m_deepestDepth = m_nodesInTree[i]->GetDepth();		
	}
}

std::vector<int> RoomTree::StartLockRooms(int _roomToLock)
{
	m_childParentNodes.clear();
	if (m_nodesInTree[_roomToLock]->m_ChildAndParent)
		m_nodesInTree[_roomToLock]->Lock(true);
	else
		lockRooms(_roomToLock);
	for (size_t i = 0; i < m_childParentNodes.size(); i++)
	{
		bool loopHasLowerDepth = m_nodesInTree[i]->GetDepth() > m_nodesInTree[_roomToLock]->GetDepth() && m_nodesInTree[i]->GetParent()->GetDepth() > m_nodesInTree[_roomToLock]->GetDepth();
		if (!loopHasLowerDepth)
		{
			int index = m_childParentNodes[i]->GetParent()->m_Index;
			while (m_nodesInTree[index]->m_Index != _roomToLock)
			{
				if (m_nodesInTree[index]->GetParent() == nullptr)
					break;
				index = m_nodesInTree[index]->GetParent()->m_Index;
			}

			unlockRooms(index, _roomToLock);
		}
	}
	std::vector<int> indexesOfInaccessibleRooms;
	for (size_t i = 0; i < m_nodesInTree.size(); i++)
	{
		if (m_nodesInTree[i]->IsLocked())
			indexesOfInaccessibleRooms.push_back(m_nodesInTree[i]->m_Index);
	}
	return indexesOfInaccessibleRooms;
	
}
int RoomTree::GetDeepestDepth()
{
	return m_deepestDepth;
}

bool RoomTree::IsRoomDepthGreater(int _roomToCheck, int _depthToCheck) 
{
	return m_nodesInTree[_roomToCheck]->GetDepth() > _depthToCheck;
}
int RoomTree::GetRoomDepth(int _room)
{
	return m_nodesInTree[_room]->GetDepth();
}
void RoomTree::unlockRooms(int _roomToUnlock, int _originLockRoom)
{	
		for (size_t i = 0; i < m_nodesInTree[_roomToUnlock]->ChildCount(); i++)
		{
				m_nodesInTree[_roomToUnlock]->GetChild(i)->Lock(false);
				m_nodesInTree[_roomToUnlock]->Lock(true);
				for (size_t i = 0; i < m_nodesInTree[_roomToUnlock]->ChildCount(); i++) {

					unlockRooms(m_nodesInTree[_roomToUnlock]->GetChild(i)->m_Index, _originLockRoom);
				}
		}
	
	
	
}

void RoomTree::lockRooms(int _roomToLock)
{
	
	if (m_nodesInTree[_roomToLock]->ShouldStopLocking())
	{
		m_childParentNodes.push_back(m_nodesInTree[_roomToLock]);
	}
	else
	{
		m_nodesInTree[_roomToLock]->Lock(true);
		for (size_t i = 0; i < m_nodesInTree[_roomToLock]->ChildCount(); i++)		{
			
			lockRooms(m_nodesInTree[_roomToLock]->GetChild(i)->m_Index);
		}
	}
	
	
}

void RoomTree::generateConnections(RoomTreeNode* _parent, const std::vector<RoomData>& _roomData)
{
	
	int currentIndex = _parent->m_Index;
	int currentParentIndex = -1;

	if (_parent->GetParent() != nullptr)
	{
		currentParentIndex = _parent->GetParent()->m_Index;
	}
	m_nodesInTree[currentIndex]->m_Checked = true;
	if (_roomData[currentIndex].sm_exitCount > 1 || currentParentIndex == -1)
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
					m_nodesInTree[currentIndex]->AddChild(m_nodesInTree[roomIndex]);
					m_nodesInTree[roomIndex]->m_ChildAndParent = true;
				}
			}
			itt++;
		}
	}
}