#pragma once
#include<vector>
#include "RoomData.h"
class RoomTreeNode;
class RoomTree
{
private:
	std::vector<RoomTreeNode*> m_nodesInTree;
	void generateConnections(RoomTreeNode* _parent, const std::vector<RoomData>& _roomData);
public:	
	RoomTree();
	~RoomTree();
	void GenerateRoomTree(const std::vector<RoomData>& _roomData, int _playerStartRoom);
	
};

