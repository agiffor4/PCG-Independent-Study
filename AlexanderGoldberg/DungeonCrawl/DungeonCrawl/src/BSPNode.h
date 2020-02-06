#pragma once
#include "RectA.h"
class BSPNode
{
public:	
	BSPNode(int x1, int y1, int x2, int y2, int _parentIndex, int _selfIndex);
	BSPNode(RectA _rect, int _parentIndex, int _selfIndex);
	~BSPNode();
	RectA GetRect();
	int GetIndex();
	int GetSelfIndex();
protected:
	RectA m_myRect = RectA();
	int m_parentIndex = -1;
	int m_selfIndex = -1;



};


