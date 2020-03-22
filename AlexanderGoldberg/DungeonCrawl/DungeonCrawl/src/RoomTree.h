#pragma once
#include<vector>
#include "RoomData.h"
class RoomTreeNode;
class RoomTree
{
private:
	std::vector<RoomTreeNode*> m_nodesInTree;
	std::vector<RoomTreeNode*> m_childParentNodes;
	void generateConnections(RoomTreeNode* _parent, const std::vector<RoomData>& _roomData);
	
	void unlockRooms(int _roomToUnlock, int _originLockRoom, std::vector<bool>& _alreadyUnlocked);
	void lockRooms(int _roomToLock);
	int m_deepestDepth = -1;
public:	
	RoomTree();
	~RoomTree();
	
	void Clear();

	void GenerateRoomTree(const std::vector<RoomData>& _roomData, int _playerStartRoom);
	std::vector<int> StartLockRooms(int _roomToLock);
	int GetDeepestDepth();
	bool IsRoomDepthGreater(int _roomToCheck, int _depthToCheck);
	bool IsRoomDepthGreaterOrEqual(int _roomToCheck, int _depthToCheck);
	bool IsRoomDepthInRange(int _roomToCheck, int _depthToCheckMin, int _depthToCheckMax);
	int GetRandomParentWithinRange(int _maxNumberOfparents, int _startingRoomIndex);

	int GetRoomDepth(int _room);
	void PrintTree();
	void PrintBranch(std::vector<RoomTreeNode*> _children, int _parentIndex, std::vector<bool>& _printedAlready);
};

