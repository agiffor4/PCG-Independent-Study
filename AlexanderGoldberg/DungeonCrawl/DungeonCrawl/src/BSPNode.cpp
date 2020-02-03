#include "BSPNode.h"

BSPNode::BSPNode(int x1, int y1, int x2, int y2, int _parentIndex, int _selfIndex)
{
	m_myRect.x1 = x1;
	m_myRect.y1 = y1;
	m_myRect.x2 = x2;
	m_myRect.y2 = y2;
	m_parentIndex = _parentIndex;
	m_selfIndex = _selfIndex;
}

BSPNode::~BSPNode()
{
}

BSPNode::RectA BSPNode::GetRect()
{
	return m_myRect;
}

int BSPNode::GetIndex()
{
	return m_parentIndex;
}

int BSPNode::GetSelfIndex()
{
	return m_selfIndex;
}
