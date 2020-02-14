#pragma once
#include <vector>
#include "RectA.h"
#include "Vector2.h"
class BSPNode;
class World;
class AStarSearch;
struct RectA;
class BSP
{
public:
	enum class DTS // DirectionToSplit
	{
		HORIZONTAL,
		VERTICAL
	};
	enum class TunnelingType {
		FirstToLast,
		RoomToRoom,
		Hub,
		Sequential,
		RegionToRegion, 
		StringOfRooms

	};
private:
	std::string m_enumNames[7] = { "FirstToLast", "RoomToRoom", "Hub", "Sequential", "RegionToRegion", "StringOfRooms", "" };
	int m_width = 0;//width of rectangle to be divided up
	int m_height = 0;//height of rectangle to be divided up
	int m_numberOfSplits = 0;
	int m_targetNumOfSplits = 0; //the number of times to split the base grid.  Resulting number of regions is equal to 2^(m_targetNumOfSplits-1)
	int m_generationAttempt = 1; //tracks how many failed generation attempts before finidng a viable generation
	bool printedOnce = false;

	bool m_ensureRoomSeperation = true; //forces each room to have a minimum one square border, if false rooms can bleed into eachother.
	bool m_usePreviouslyDugPathsInPathGeneration = true; //if false each path is dug in isolation of the other paths, if true each path takes into account the previously dug paths.

	std::vector<BSPNode*> m_tree;
	std::vector<DTS> m_previousRotations;
	std::vector<RectA> m_roomRegions;
	std::vector<int> m_usablePaths;

	TunnelingType m_tunnelingType = TunnelingType::FirstToLast;
	

	void split();
	void WipeTree();
	void StartOver();

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
	std::vector<std::vector<int>> GetPartions();
	std::vector<std::vector<int>> GetRoomTileIndexes();
	void GenerateRoomsAndPaths(AStarSearch& _AStar, std::vector<std::vector<int>>& _generatedRooms, std::vector<int>& _generatedPaths);
	std::vector<RectA> GetRoomRegions(bool _overwritePreviousRooms = false);
	std::vector<int> GeneratePaths(AStarSearch& _AStar, bool _overwritePreviousPaths = false, std::vector<std::vector<int>>* const _roomTileIndexes = nullptr);
	int convertXYToIndex(int _x, int _y, int _width);
	const Vector2 convertIndexToXY(int _index, int _width);
	void TunnelingWorkInwards(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);
	void TunnelingRoomToRoom(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _repeatRoomDigs, bool _updateMapWithPreviousPaths);
	void TunnelingSpiderOut(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths, bool _randomizeWhichRoomIsOrigin = true, int _centralRoomToSpiralFrom = 0);
	void TunnelingSequential(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);
	void TunnelingRegionToRegion(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);
	void TunnelingCorridorsThroughRooms(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);
	const std::string& GetEnumName(TunnelingType _enumValueToGetNameOf);
	int Size();

	void SetTunnelingType(TunnelingType _tunnelingType);
	
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