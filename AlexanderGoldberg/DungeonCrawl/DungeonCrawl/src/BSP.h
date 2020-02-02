#pragma once
#include <vector>
class BSP
{
private:
	int m_width = 0;
	int m_height = 0;
	int m_currentHead = 0;
	
public:
	enum class DTS // DirectionToSplit
	{
		HORIZONTAL,
		VERTICAL
	};
	BSP(int _gridWidth, int _gridHeight);
	~BSP();
	void Split(DTS _direction);
	void Split(DTS _direction);
	struct RectA
	{
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
	};
};

