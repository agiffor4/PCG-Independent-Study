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
		Base,		
		Hub,
		StringOfRooms,
		RoomToRoom,

		RegionToRegion, 

	};
private:
	std::string m_enumNames[7] = { "Base", "Hub", "StringOfRooms", "RoomToRoom", "RegionToRegion", "other", "" };
	int m_width = 0;//width of rectangle to be divided up
	int m_height = 0;//height of rectangle to be divided up
	int m_numberOfSplits = 0;
	int m_targetNumOfSplits = 0; //the number of times to split the base grid.  Resulting number of regions is equal to 2^(m_targetNumOfSplits-1)
	int m_generationAttempt = 1; //tracks how many failed generation attempts before finidng a viable generation
	bool printedOnce = false;

	bool m_ensureRoomSeperation = true; //forces each room to have a minimum one square border, if false rooms will bleed into eachother creating larger open areas.
	bool m_usePreviouslyDugPathsInPathGeneration = true; //if false each path is dug in isolation of the other paths, if true each path takes into account the previously dug paths.

	std::vector<BSPNode*> m_tree;
	std::vector<DTS> m_previousRotations;
	std::vector<RectA> m_roomRegions;
	std::vector<int> m_usablePaths;
	std::vector<Vector2> m_IndexesOfStartEndPointsForPathSegments; //the indexs of start and end points for the paths withing m_usablePath. e.g. m_usablePaths[m_IndexesOfStartEndPointsForPathSegments[1].x] is the start tile index of the path and m_usablePaths[m_IndexesOfStartEndPointsForPathSegments[1].y] is the end tile index of the second path

	TunnelingType m_tunnelingType = TunnelingType::Base;
	

	void split();
	void WipeTree();
	void StartOver();

	int RoomIndexTileIsIn(int _tileIndex);
	bool IsIndexInRoom(int _tileIndex, int _roomIndex);

	void print(std::vector<int>& _toPrint, int _width);	
	void printLeafResults();

public:
	BSP(int _gridWidth, int _gridHeight);
	~BSP();
	
	BSPNode* GetFirstLeaf();
	int GetFirstLeafIndex();
	std::vector<BSPNode*> GetLeaves();
	BSPNode* GetRandomLeaf();
	
	//AREA DIVSION
	void BeginSplit(int _timesToSplit);
	void GenerateRoomsAndPaths(AStarSearch& _AStar, std::vector<std::vector<int>>& _generatedRooms, std::vector<int>& _generatedPaths);
	std::vector<std::vector<int>> GetPartions();
	std::vector<std::vector<int>> GetRoomTileIndexes();
	std::vector<RectA> GetRoomRegions(bool _overwritePreviousRooms = false);
	std::vector<int> GeneratePaths(AStarSearch& _AStar, bool _overwritePreviousPaths = false, std::vector<std::vector<int>>* const _roomTileIndexes = nullptr);


	
	//EXIT LOCATION SETTING FUNCTION
	int GenerateExitLocation(int _startingIndex, int _startingRegion, World& _world); //calls function based on alg used to generate paths
	void furthestExitAsTheCrowFlies(int& _exitIndex, int _startingRegion, Vector2& const _startingPosition);
	void longestPathToFromStart(int& _exitIndex, int _startingIndex, World& _world);

	//PATH GENERATION FUNCTIONS
	void SetTunnelingType(TunnelingType _tunnelingType);
	void TunnelingBase(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);
	int TunnelingHub(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths, bool _randomizeWhichRoomIsOrigin = true, bool _tryToPickCenteralRoom = true, int _centralRoomToSpiralFrom = 0); // returns "hub" room index
	void TunnelingStringsOfRooms(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);
	void TunnelingRoomToRoom(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _repeatRoomDigs = true, bool _updateMapWithPreviousPaths = true);	
	void TunnelingRegionToRegion(AStarSearch& _AStar, std::vector<std::vector<int>>& const indexesOfRoomTiles, bool _updateMapWithPreviousPaths);

	std::vector<std::vector<int>> GetCorridorOnlyTiles();

	//UTILITY
	int convertXYToIndex(int _x, int _y, int _width);
	const Vector2 convertIndexToXY(int _index, int _width);
	const std::string& GetEnumName(TunnelingType _enumValueToGetNameOf);
	int getRandomTileInRoom(int _roomRegion);
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

std::vector<std::vector<int>> GetCorridorOnlyTiles();
