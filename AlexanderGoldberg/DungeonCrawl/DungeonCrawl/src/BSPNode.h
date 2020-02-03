#pragma once
class BSPNode
{
public:
	struct RectA
	{
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
	};
	BSPNode(int x1, int y1, int x2, int y2, int _parentIndex, int _selfIndex);
	~BSPNode();
	RectA GetRect();
	int GetIndex();
	int GetSelfIndex();
protected:
	RectA m_myRect = RectA();
	int m_parentIndex = -1;
	int m_selfIndex = -1;



};


