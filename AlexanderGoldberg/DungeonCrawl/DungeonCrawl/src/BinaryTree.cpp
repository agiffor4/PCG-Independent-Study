#include "BinaryTree.h"
#include "TreeNode.h"

BinaryTree::BinaryTree() 
{
	m_root = new TreeNode(nullptr, "Root");
	m_size = 1;
	m_leaves.push_back(m_root);
		
}
BinaryTree::~BinaryTree() 
{
	if (m_root != nullptr)
	{
		delete m_root;
		m_root = nullptr;
	}
	
}

int BinaryTree::size()
{
	return m_size;
}

void BinaryTree::Add()
{	

	std::vector<TreeNode*> newLeaves;
	
	for (size_t i = 0; i < m_leaves.size(); i++)
	{
		
		m_leaves[i]->Add(m_leaves[i], m_size);
		m_size += 2;
		newLeaves.push_back(m_leaves[i]->GetLeft());
		newLeaves.push_back(m_leaves[i]->GetRight());
		
	}
	m_leaves.erase(m_leaves.begin(), m_leaves.end());
	m_leaves = newLeaves;

}


void BinaryTree::Split(DTS _splitDirection)
{
	/*
	treeNode
rec (x1, y1, x2, y2)
LNode
RNode


if vertical
split point = random point between root.x1 and root.x2
rhs.x1 = split point
rhs.y1 =  root.y1
rhs.x2 = root.x2
rhs.y2 = root.y2

lhs.x1 = root.x1
lhs.y1 =  root.y1
lhs.x2 = split point
lhs.y2 = root.y2


if horizontal
split point = random point between root.y1 and root.y2
rhs.x1 = root.x1
rhs.y1 =  split point
rhs.x2 = root.x2
rhs.y2 = root.y2

lhs.x1 = root.x1
lhs.y1 =  root.y1
lhs.x2 = root.x2
lhs.y2 = split point
	*/
}