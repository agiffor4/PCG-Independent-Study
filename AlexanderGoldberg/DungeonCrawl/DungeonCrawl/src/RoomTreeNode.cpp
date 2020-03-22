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
	_child->SetDepth(m_depth + 1);
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
	if (m_branches.size() > 0 && _index > -1 && _index < m_branches.size())
	{
		return m_branches[_index];
	}
	return nullptr;
	
}

void RoomTreeNode::Lock(bool _val)
{
	m_locked = _val;
}

bool RoomTreeNode::IsLocked()
{
	return m_locked;
}

void RoomTreeNode::SetDepth(int _depth)
{
	m_depth = _depth;
}

int RoomTreeNode::GetDepth()
{
	return m_depth;
}

bool RoomTreeNode::ShouldStopLocking()
{
	
	if (m_ChildAndParent)
		return true;
	return false;
}

const std::vector<RoomTreeNode*>& RoomTreeNode::GetChildren()
{
	return m_branches;
}

