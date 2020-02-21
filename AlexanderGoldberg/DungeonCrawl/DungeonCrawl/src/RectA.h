#pragma once
class RectA
{
public:
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	RectA();
	RectA(int _x1, int _y1, int _x2, int _y2);
	void Set(int _x1, int _y1, int _x2, int _y2);
	bool CheckIfMeetsOrExceedsMin(int _minWidth, int _minHeight);
	int Area();
	bool Contains(int _x, int _y);
	void GetCenter(int* _x, int* _y);
	void GetCenter(double* _x, double* _y);
	int GetWidth();
	int GetHeight();
};

