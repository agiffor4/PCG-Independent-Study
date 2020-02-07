#include "AStarNode.h"
AStarNode::AStarNode()
{
}
AStarNode::~AStarNode()
{
}
void AStarNode::Init(int _currentIndex, int _xIndex, int _yIndex) {
	m_posInVector = _currentIndex;
	m_posInGrid.X = _xIndex;
	m_posInGrid.Y = _yIndex;

}
void AStarNode::SetPassable(bool _val) {
	m_passable = _val;

}
bool AStarNode::IsPassable() { return m_passable; }


int AStarNode::GetPositionInVector() { return m_posInVector; }

