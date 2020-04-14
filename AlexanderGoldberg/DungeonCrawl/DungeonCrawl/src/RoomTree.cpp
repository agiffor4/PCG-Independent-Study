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
	m_root = _playerStartRoom;
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
			std::vector<bool> alreadyChecked;
			alreadyChecked.reserve(m_nodesInTree.size());
			alreadyChecked.assign(alreadyChecked.capacity(), false);
			unlockRooms(index, _roomToLock, alreadyChecked);
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
bool RoomTree::IsRoomDepthGreaterOrEqual(int _roomToCheck, int _depthToCheck)
{
	return m_nodesInTree[_roomToCheck]->GetDepth() >= _depthToCheck;
}
bool RoomTree::IsRoomDepthInRange(int _roomToCheck, int _depthToCheckMin, int _depthToCheckMax)
{
	return m_nodesInTree[_roomToCheck]->GetDepth() >= _depthToCheckMin && m_nodesInTree[_roomToCheck]->GetDepth() < _depthToCheckMax;
}
int RoomTree::GetRandomParentWithinRange(int _maxNumberOfparents, int _startingRoomIndex)
{
	int parentsUp = -1;
	while (parentsUp > m_nodesInTree.size() -1 || parentsUp < 0)
	{
		parentsUp = _maxNumberOfparents == 0 ? 1 : (rand() % _maxNumberOfparents) + 1;
		if (parentsUp < 0)
			parentsUp = 0;
	}
	int toReturn = _startingRoomIndex;
	for (size_t i = 0; i < parentsUp; i++)
	{
		toReturn = m_nodesInTree[toReturn]->m_Index;
	}
	return toReturn;
}
int RoomTree::GetFirstChildOfRoomWithTreePosition(int _parentRoom)
{
	if (_parentRoom > m_nodesInTree.size() - 1)
		return -1;
	if (m_nodesInTree[_parentRoom]->ChildCount() < 1)
		return -1;
	return m_nodesInTree[_parentRoom]->GetChild(0)->m_Index;
}
int RoomTree::GetRoomDepth(int _room)
{
	return m_nodesInTree[_room]->GetDepth();
}
int RoomTree::GetRootIndexInTree()
{
	return m_root;
}
void RoomTree::PrintTree()
{
	RoomTreeNode* root = nullptr;
	for (size_t i = 0; i < m_nodesInTree.size(); i++)
	{
		if (m_nodesInTree[i]->GetParent() == nullptr)
		{
			root = m_nodesInTree[i];
			break;
		}	
	}
	std::vector<bool> printedChildParent;
	printedChildParent.reserve(m_nodesInTree.size());
	for (size_t i = 0; i < m_nodesInTree.size(); i++)
	{
		printedChildParent.push_back(false);
	}
	printf("__________________________\n");
	PrintBranch(root->GetChildren(), root->m_Index, printedChildParent);
	printf("__________________________\n");


}

void RoomTree::PrintBranch(std::vector<RoomTreeNode*> _children, int _parentIndex, std::vector<bool>& _printedAlready)
{
	if (_children.size() > 0)
	{
		printf("%d[%d]<", _parentIndex, m_nodesInTree[_parentIndex]->GetDepth());
		for (size_t i = 0; i < _children.size(); i++)
		{
			printf("%d", _children[i]->m_Index);
			if (i < _children.size() - 1)
			{
				if (_children[i]->m_ChildAndParent)
				{
					printf("*, ");
					
				}
				else
				{
					printf(", ");
				}
			}
			else
			{
				if (_children[i]->m_ChildAndParent)
				{
					printf("*>\n");
				}
				else
				{
					printf(">\n");
				}
				
			}
		}
		for (size_t i = 0; i < _children.size(); i++)
		{
			if (!_printedAlready[_children[i]->m_Index])
			{
				if (_children[i]->m_ChildAndParent)
					_printedAlready[_children[i]->m_Index] = true;
				PrintBranch(_children[i]->GetChildren(), _children[i]->m_Index, _printedAlready);
			}
			
		}
	}
	else
	{
		printf("%d<>\n", _parentIndex);
	}
}


void RoomTree::unlockRooms(int _roomToUnlock, int _originLockRoom, std::vector<bool>& _alreadyUnlocked)
{	
		for (size_t i = 0; i < m_nodesInTree[_roomToUnlock]->ChildCount(); i++)
		{
				_alreadyUnlocked[_roomToUnlock] = true;
				m_nodesInTree[_roomToUnlock]->GetChild(i)->Lock(false);
				for (size_t i = 0; i < m_nodesInTree[_roomToUnlock]->ChildCount(); i++) 
				{
					if (!_alreadyUnlocked[m_nodesInTree[_roomToUnlock]->GetChild(i)->m_Index])
					{
						unlockRooms(m_nodesInTree[_roomToUnlock]->GetChild(i)->m_Index, _originLockRoom, _alreadyUnlocked);
					}
					
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
					if (m_nodesInTree[currentParentIndex]->GetDepth() < m_nodesInTree[currentIndex]->GetDepth()-1)
						m_nodesInTree[currentIndex]->SetDepth(m_nodesInTree[currentParentIndex]->GetDepth() + 1);
					m_nodesInTree[roomIndex]->m_ChildAndParent = true;
				}
			}
			itt++;
		}
	}
}