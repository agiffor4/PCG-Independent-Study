#pragma once
#include "Holdable.h"
#include <vector>
#include <set>
#include "IInputHandler.h"
class Tile;
class World;
class Light :
	public Holdable, public IInputHandler
{
protected:
	int m_lightRadiusInTiles = 1;
	World* m_world = nullptr;
public:
	void SetLightRadius(int _lightRadiusInTiles);
	void SetWorld(World* _world);
	void Illuminate(bool _lightUp);
	void SetLocation(Tile* _newLocation) override;
	std::set<Tile*> GetEffectedTiles(Tile* _epicenter);
	bool Interaction() override;
	void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	Light();
	~Light();
};

