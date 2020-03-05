#include "RoomTreeNode.h"


RoomTreeNode::RoomTreeNode(RoomTreeNode* _parent) {
	m_parent = _parent;

}
RoomTreeNode::~RoomTreeNode() {

	
}

void RoomTreeNode::SetParent(RoomTreeNode* _parent)
{
	m_parent = _parent;
}

RoomTreeNode* RoomTreeNode::GetParent()
{
	return m_parent;
}

void RoomTreeNode::AddChild(RoomTreeNode* _child)
{
	m_branches.push_back(_child);
}

bool RoomTreeNode::IsRoot()
{
	return m_parent == nullptr;
}

int RoomTreeNode::ChildCount()
{
	return m_branches.size();
}

RoomTreeNode* RoomTreeNode::GetChild(int _index)
{
	return m_branches[_index];
}

void RoomTreeNode::Lock(bool _val)
{
	m_locked = _val;
}

bool RoomTreeNode::IsLocked()
{
	return m_locked;
}

bool RoomTreeNode::ShouldStopLocking()
{
	
	if (m_ChildAndParent)
		return true;
	for (size_t i = 0; i < m_branches.size(); i++)
	{
		if (m_branches[i]->m_ChildAndParent)
			return true;
	}

	return false;
}

