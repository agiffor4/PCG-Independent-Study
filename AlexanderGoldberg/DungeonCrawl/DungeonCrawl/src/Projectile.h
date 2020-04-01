#pragma once
#include "Thing.h"
#include "Vector2.h"
class Projectile :
	public Thing
{
private:
	int m_speedMultiplier = 100;

protected:
	Vector2 m_direction;
	int m_damage = 1;
	int m_speed = 1;
	void move(float _dt);
	Thing* m_owner = nullptr;
public:
	Projectile();
	~Projectile();
	void OnSpawn(const Vector2& _spawnPosition, const Vector2& _direction, float _speed, int _damage, Thing* _owner);
	void Update(float _dt) override;
	void OnCollision(Thing* _other) override;
};

