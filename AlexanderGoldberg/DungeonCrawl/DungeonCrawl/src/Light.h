#pragma once
#include "Holdable.h"
#include <vector>
#include <set>
class Tile;
class World;
class Light :
	public Holdable
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
	Light();
	~Light();
};

