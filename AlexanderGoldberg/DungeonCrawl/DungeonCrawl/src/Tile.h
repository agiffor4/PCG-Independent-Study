#pragma once
#include "Renderable.h"
#include "IInputHandler.h"
#include <vector>
class Interactable;
class Thing;
class World;

class Tile :
	public Renderable, IInputHandler
{
private:
	Thing* m_contents = nullptr;
	std::vector<Interactable*> m_items;
	Vector2 m_posInGrid = Vector2();
	World* m_world;
	int m_posInVector = -1;
	bool m_passable = true;
	bool m_corridor = false;
public:


	Tile();
	~Tile();
	void Init(const std::string _path, const std::string _name, int _currentIndex, int _xIndex, int _yIndex, Vector2 _startPos, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	bool IsPassable();
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
	const Vector2& GetPositionInGrid();
	void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	void ClearTileContents();
	void Render(SDL_Renderer* _renderer) override;
	
};

