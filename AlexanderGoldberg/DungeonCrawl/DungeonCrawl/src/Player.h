#pragma once
#include "WeaponHolder.h"
#include "IInputHandler.h"
#include "World.h"
#include "Timer.h"
#include "MouseAim.h"
class PlayerHUD;
class Weapon;
class Scene;
class Interactable;
class Holdable;
class WeaponNameRenderer;
class Player :
	public WeaponHolder, public IInputHandler
{
	
public:	
	Player();
	~Player();
	void Update(float _dt) override;
	void InvokeKeyDown(SDL_Keycode _key) override;
	void InvokeKeyUp(SDL_Keycode _key) override;
	void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	void InvokeMouseDown(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	void Initalize(World& _world, const std::string _path, const std::string _name, Scene* _scene, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	void SetLineOfSight(bool _inLineOfSight);
	bool AddAmmo(int _amount);
	void Render(SDL_Renderer* _renderer)override;
	void EquipWeapon(Weapon* _weapon);
	int GetAmmo();
	float GetAmmoAsPercent();
	int GetLevelGenCount();
protected:

	WeaponNameRenderer* m_nameRenderer = nullptr;
	bool m_fireButtonDown = false;
	PlayerHUD* m_hud = nullptr;
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
	MouseAim m_mouseAim = MouseAim();
	World* m_world;
	std::vector<Thing*> m_inventory;
	int m_inventoryMax = -1;
	int m_lineOfSightRadius = 3;
	Holdable* m_holdables[2];
	Weapon* m_equipedWeapon = nullptr;
	
	void InteractWithThingInSpace();
	void attack(Vector2 _direction);
	void move(Vector2 _direction);
	World::TileDirection convertVectorToDirection(Vector2& const _toConvert);
	void setDirectionFromFlagValues(Uint8 _flag);
	void dropHoldable();
	bool dropWeapon();
	Tile* findEmptyAdjacentSpace();
	std::set<Tile*> getTilesInLineOfSight(Tile* _epicenter);
	void die() override;
};
