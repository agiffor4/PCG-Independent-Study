#pragma once
#include <vector>
#include <set>
class RoomData
{

	
public:
	std::vector<int> sm_containsTiles;
	int sm_connectedness = 0;
	int sm_exitCount = 0;
	std::set<int> sm_regionsExitingTo;
	std::set<int> sm_CorridorExits;
	int sm_region = -1;
	bool sm_Locked = false;
	bool Contains(int _index);
		
};

