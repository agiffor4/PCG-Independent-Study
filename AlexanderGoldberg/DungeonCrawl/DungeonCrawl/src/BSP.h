#pragma once
#include <vector>
#include "RectA.h"
class BSPNode;
class World;
struct RectA;
class BSP
{
public:
	enum class DTS // DirectionToSplit
	{
		HORIZONTAL,
		VERTICAL
	};
private:

	int m_width = 0;
	int m_height = 0;
	int m_currentHead = 0;
	int m_numberOfSplits = 0;
	int m_targetNumOfSplits = 0;
	int m_generationAttempt = 1;
	bool printedOnce = false;
	bool m_ensureRoomSeperation = true;
	std::vector<BSPNode*> m_tree;
	std::vector<DTS> m_previousRotations;
	void split();
	void WipeTree();
	void StartOver();
	std::vector<RectA> generateRooms();
	std::vector<int> generatePaths();
	void print(std::vector<int>& _toPrint, int _width);	
	void printLeafResults();

public:
	BSP(int _gridWidth, int _gridHeight);
	~BSP();
	void BeginSplit(int _timesToSplit);
	BSPNode* GetFirstLeaf();
	int GetFirstLeafIndex();
	std::vector<BSPNode*> GetLeaves();
	BSPNode* GetRandomLeaf();
	std::vector<std::vector<int>> GetPartions(World* _world);
	std::vector<std::vector<int>> GetRooms(World* _world);
	int Size();

	
};

/*
		 if vertical
		split point = random point between root.x1 and root.x2
		rhs.x1 = split point
		rhs.y1 =  root.y1
		rhs.x2 = root.x2
		rhs.y2 = root.y2

		lhs.x1 = root.x1
		lhs.y1 =  root.y1
		lhs.x2 = split point
		lhs.y2 = root.y2
			if horizontal
				split point = random point between root.y1 and root.y2
				rhs.x1 = root.x1
				rhs.y1 = split point
				rhs.x2 = root.x2
				rhs.y2 = root.y2

				lhs.x1 = root.x1
				lhs.y1 = root.y1
				lhs.x2 = root.x2
				lhs.y2 = split point*/