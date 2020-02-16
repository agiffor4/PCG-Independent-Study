#pragma once
#include "Vector2.h"
class AStarNode
{
private:
	Vector2 m_posInGrid = Vector2();
	int m_posInVector = -1;
	bool m_passable = true;
	bool m_corridor = false;
public:
	int m_GCost = 1;

	AStarNode();
	~AStarNode();
	void Init(int _currentIndex, int _xIndex, int _yIndex);
	bool IsPassable();
	void SetPassable(bool _val);
	bool IsCorridor();
	void SetCorridor(bool _val);
	int GetPositionInVector();
	Vector2 GetPositionInGrid() {
		return m_posInGrid;
	}
};

