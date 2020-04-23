#include "RoomData.h"
bool RoomData::Contains(int _index) {
	if (_index < sm_containsTiles[0] || _index > sm_containsTiles[sm_containsTiles.size() - 1])
		return false;
	for (size_t i = 0; i < sm_containsTiles.size(); i++)
	{
		if (_index == sm_containsTiles[i])
			return true;
	}
	return false;
}

int RoomData::GetRandomTile()
{
	return sm_containsTiles[rand() % sm_containsTiles.size()];
}

std::vector<int> RoomData::GetCorners()
{
	std::vector<int> corners;
	corners.push_back(sm_containsTiles[sm_containsTiles.size() - 1] - sm_width);
	corners.push_back(sm_containsTiles[0]);
	corners.push_back(sm_containsTiles[0] + sm_width);
	corners.push_back(sm_containsTiles[sm_containsTiles.size()-1]);

	return corners;
}

