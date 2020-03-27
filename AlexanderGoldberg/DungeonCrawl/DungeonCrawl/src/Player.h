#pragma once
#include "Damagable.h"
#include "IInputHandler.h"
#include "World.h"
#include "Timer.h"
class Scene;
class Interactable;
class Holdable;
class Player :
	public Damagable, public IInputHandler
{

public:	
	Player();
	~Player();
	void Update(float _dt) override;
	void InvokeKeyDown(SDL_Keycode _key) override;
	void InvokeKeyUp(SDL_Keycode _key) override;
	void Initalize(World& _world, const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	void SetLineOfSight(bool _inLineOfSight);
protected:
	
	//MOVEMENT VARIABLES
	enum class MovementDirection {
		LEFT = 1,
		RIGHT = 2,
		UP = 4,
		DOWN = 8

	};
	Uint8 m_directionKeyDownFlag = 0;
	Vector2 m_direction = Vector2();
	Timer m_moveRate = Timer(0.5f);
	World* m_world;
	std::vector<Thing*> m_inventory;
	int m_inventoryMax = -1;
	int m_lineOfSightRadius = 2;
	Holdable* m_holdables[2];
	void InteractWithThingInSpace();
	void attack(Vector2 _direction);
	void move(Vector2 _direction);
	World::TileDirection convertVectorToDirection(Vector2& const _toConvert);
	void setDirectionFromFlagValues(Uint8 _flag);
	
	std::set<Tile*> getTilesInLineOfSight(Tile* _epicenter);
	void die() override;
};
