#pragma once
#include "Renderable.h"
#include "IInputHandler.h"
#include <vector>
class Interactable;
class Thing;
class World;
class TextA;
class Tile :
	public Renderable, IInputHandler
{
private:
	Thing* m_contents = nullptr;
	std::vector<Interactable*> m_items;
	Vector2 m_posInGrid = Vector2();
	int m_posInVector = -1;
	bool m_passable = true;
	bool m_corridor = false;
	int m_roomIn = -1;
	TextA* m_text = nullptr;
	enum class TileRenderType {
		empty,
		wall4Side, 
		/*
		 x
		x0x
		 x
		*/
		wall3Sidetop,
		/*
		 x
		x0x
		 o
		*/
		wall3SideBottom,
		/*
		 o
		x0x
		 x
		*/
		wall2SideTopL,
		/*
		 x
		x0o
		 o
		*/
		wall2SideTopR,
		/*
		 x
		o0x
		 o
		*/
		wall2SideBottomL,
		/*
		 o
		x0o
		 x
		*/
		wall2SideBottomR,
		/*
		 o
		o0x
		 x
		*/
		wall3SideLeft,
		/*
		 x
		x0o
		 x
		*/
		wall3SideRight,
		/*
		 x
		o0x
		 x
		*/
		wall1SideBottom,
		/*		
		 o
		o0o
		 x			
		*/
		wall1SideTop,
		/*
		 x
		o0o
		 o
		*/
		wall1SideLeft,
		/*
		 o
		x0o
		 o
		*/
		wall1SideRight,
		/*
		 o
		o0x
		 o
		*/
		wall2SideLeftRight,
		/*
		 o
		x0x
		 o
		*/
		wall2SideUpDown
		/*
		 x
		o0o
		 x
		*/

	};
	TileRenderType m_tilerRenderType = TileRenderType::empty;
	enum class WallNeigbors {
		above = 1,
		below = 2,
		right = 4,
		left = 8,

	};
public:


	Tile();
	~Tile();
	void Init(const std::string _path, const std::string _name, int _currentIndex, int _xIndex, int _yIndex, Vector2 _startPos, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	bool IsPassable(bool _ignoreInteractables = false);
	bool blockingInteractable();
	bool inBounds(int _x, int _y);
	void SetPassable(bool _val);
	void SetCorridor(bool _val);
	bool IsCorridor();
	void SetContents(Thing* _newContents);
	Thing* GetContents();
	void MoveContentsTo(Tile* _newLocation);
	int GetPositionInVector();
	Interactable* InteractWithItem();
	void AddItem(Interactable* _newItem);
	Interactable* RemoveItem(Interactable* _toRemove, bool _deleteOnRemoval);
	const Vector2& GetPositionInGrid();
	void AddRoomNumber(int _roomTileIsIn, bool _renderNumbers, SDL_Renderer* _renderer);
	void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	void PrintTileData();
	void ClearTileContents();
	void Render(SDL_Renderer* _renderer) override;
	void Update(float _dt) override;
	void DetermineTileType(World* _world);
};

