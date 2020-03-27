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
	void Init(const std::string _path, const std::string _name, SDL_Renderer* _renderer, World* _world, Uint32 _transparentColor = -999);
	std::set<Tile*> GetEffectedTiles(Tile* _epicenter);
	bool Interaction(Thing* _thingInitatingInteraction) override;
	Light();
	~Light();
};

