#pragma once
#include "Thing.h"
#include "Vector2.h"
#include <vector>
#include <set>
#include "Weapon.h"
#include "WeaponStructs.h"
class Enemy;
class Tile;
class World;
class Projectile :
	public Thing
{
private:
	int m_speedMultiplier = 100;

protected:
	Vector2 m_direction;
	int m_damage = 1;
	int m_speed = 1;
	Uint32 m_weaponProperties = 0;
	Tile* m_currentTileIn = nullptr;
	void move(float _dt);
	Thing* m_owner = nullptr;
	Enemy* m_homingTarget =  nullptr;
	bool hasProperty(Weapon::weaponProperties _property);
	WeaponStructs::ProjectileStruct m_projectileData;
	WeaponStructs::AOEStruct m_aoeData;
	WeaponStructs::LightStruct m_lightData;
	World* m_world = nullptr;
	void die();
public:
	Projectile();
	~Projectile();
	void OnSpawn(const Vector2& _spawnPosition, const Vector2& _direction, float _speed, int _damage, Uint32 _weaponProfile,Thing* _owner);
	void SetStructData(World* _world, WeaponStructs::ProjectileStruct _projectileData, WeaponStructs::AOEStruct _aoeData, WeaponStructs::LightStruct _lightData);
	void Update(float _dt) override;
	void OnCollision(Thing* _other) override;
	void SetHomingTarget(Enemy* _toSet);
	bool HomingTargetIsValid();
	std::set<Tile*> GetEffectedTiles(Tile* _epicenter);
	void Illuminate(bool _lightUp);
};


