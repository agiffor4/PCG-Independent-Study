#pragma once
#include "Thing.h"
#include "IInputHandler.h"
#include "World.h"
class Scene;
class Player :
	public Thing, public IInputHandler
{

public:
	Player();
	~Player();
	void InvokeKeyDown(SDL_Keycode _key) override;
	void InvokeKeyUp(SDL_Keycode _key) override;
	void Initalize(World& _world, const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
protected:
	World* m_world;
	void attack(Vector2 _direction);
	void move(Vector2 _direction);
	World::TileDirection convertVectorToDirection(Vector2& const _toConvert);
};

