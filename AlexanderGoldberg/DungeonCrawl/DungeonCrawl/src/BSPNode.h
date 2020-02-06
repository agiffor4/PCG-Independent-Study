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
		RectA() {}
		RectA(int _x1, int _y1, int _x2, int _y2) {
			x1 = _x1;
			y1 = _y1;
			x2 = _x2;
			y2 = _y2;
		}
		void Set(int _x1, int _y1, int _x2, int _y2) {
			x1 = _x1;
			y1 = _y1;
			x2 = _x2;
			y2 = _y2;
		}
		bool CheckIfMeetsOrExceedsMin(int _minWidth, int _minHeight) {
			int width = x1 - x2;
			if (width < 0)
				width *= -1;
			int height = y1 - y2;
			if (height < 0)
				height *= -1;
			return width >= _minWidth && height >= _minHeight;
		}
		int Area() {
			int width = x1 - x2;
			if (width < 0)
				width *= -1;
			int height = y1 - y2;
			if (height < 0)
				height *= -1;
			return width * height;
		}
	};
	BSPNode(int x1, int y1, int x2, int y2, int _parentIndex, int _selfIndex);
	BSPNode(BSPNode::RectA _rect, int _parentIndex, int _selfIndex);
	~BSPNode();
	RectA GetRect();
	int GetIndex();
	int GetSelfIndex();
protected:
	RectA m_myRect = RectA();
	int m_parentIndex = -1;
	int m_selfIndex = -1;



};


