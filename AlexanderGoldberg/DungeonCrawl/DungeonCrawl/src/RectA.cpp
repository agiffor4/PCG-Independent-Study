#include "RectA.h"
RectA::RectA() {}
RectA::RectA(int _x1, int _y1, int _x2, int _y2) {
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
}
void RectA::Set(int _x1, int _y1, int _x2, int _y2) {
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
}
bool RectA::CheckIfMeetsOrExceedsMin(int _minWidth, int _minHeight) {
	int width = x1 - x2;
	if (width < 0)
		width *= -1;
	int height = y1 - y2;
	if (height < 0)
		height *= -1;
	return width >= _minWidth && height >= _minHeight;
}
int RectA::Area() {
	int width = x1 - x2;
	if (width < 0)
		width *= -1;
	int height = y1 - y2;
	if (height < 0)
		height *= -1;
	return width * height;
}