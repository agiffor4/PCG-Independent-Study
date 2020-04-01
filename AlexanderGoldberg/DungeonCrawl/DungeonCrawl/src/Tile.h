#pragma once
#include "Thing.h"
#include "IInputHandler.h"
#include <vector>
class Interactable;
class World;
class TextA;
class Shadow;
class Tile :
	public Thing, IInputHandler
{
private:
	Thing* m_contents = nullptr;
	std::vector<Interactable*> m_items;
	std::vector<Shadow*> m_shadows;
	Vector2 m_posInGrid = Vector2();
	int m_posInVector = -1;
	bool m_passable = true;
	bool m_corridor = false;
	bool m_illuminated = false;
	int m_distanceFromLightsource = -1;
	bool m_inFogOfWar = true;
	bool m_renderFogOfWar = true;
	int m_distanceFromSource = -1;
	int m_roomIn = -1;
	TextA* m_text = nullptr;
	int m_lightModifer = 30;
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
		/*
		ooo
		o0o
		oox
		*/

	};
	TileRenderType m_tilerRenderType = TileRenderType::empty;
	enum class WallNeigbors {
		above = 1,
		below = 2,
		right = 4,
		left = 8,
		northeast = 16,
		northwest = 32,
		southeast = 64,
		southwest = 128

	};
	bool m_clickAble = false;
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
	void AddShadow(Shadow* _newShadow);
	Interactable* RemoveItem(Interactable* _toRemove, bool _deleteOnRemoval);
	const Vector2& GetPositionInGrid();
	void AddRoomNumber(int _roomTileIsIn, bool _renderNumbers, SDL_Renderer* _renderer);
	void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	void InvokeKeyUp(SDL_Keycode _key) override;
	void PrintTileData();
	void ClearTileContents();
	void Render(SDL_Renderer* _renderer) override;
	void Update(float _dt) override;
	void DetermineTileType(World* _world);
	void SetIlluminated(bool _illuminated, int _distanceFromLightsource);
	void SetFogOfWar(bool _inFogOfWar, int _distanceFromSource);
	const std::vector<Interactable*>& GetItems();

};

