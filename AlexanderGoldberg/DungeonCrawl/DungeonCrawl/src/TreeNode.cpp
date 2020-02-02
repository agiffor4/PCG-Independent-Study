#include "TreeNode.h"


TreeNode::TreeNode(TreeNode* _parent, std::string _name) {
	m_name = _name;
	m_parent = _parent;
}
TreeNode::~TreeNode() {

	if (m_LTreeNode != nullptr)
	{
		delete(m_LTreeNode);
		m_LTreeNode = nullptr;
	}
	if (m_RTreeNode != nullptr)
	{
		delete(m_RTreeNode);
		m_RTreeNode = nullptr;
	}
}


void TreeNode::Add(TreeNode* _parent, int _number)
{
	m_LTreeNode = new TreeNode(_parent, "child node " + std::to_string(_number + 1));
	m_RTreeNode = new TreeNode(_parent, "child node " + std::to_string(_number + 2));
}

TreeNode* TreeNode::GetLeft()
{
	return m_LTreeNode;
}

TreeNode* TreeNode::GetRight()
{
	return m_RTreeNode;
}
