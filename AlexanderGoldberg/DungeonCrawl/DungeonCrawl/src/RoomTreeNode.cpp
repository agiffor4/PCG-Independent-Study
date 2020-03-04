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

