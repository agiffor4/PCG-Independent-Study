#include "Projectile.h"
#include "Damagable.h"
void Projectile::move(float _dt)
{
	if (m_checkCollision)
	{
		Vector2 pos = GetPosition() + (m_direction * m_speed * m_speedMultiplier * _dt);
		SetPosition(pos);
	}
}

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::OnSpawn(const Vector2& _spawnPosition, const Vector2& _direction, float _speed, int _damage, Thing* _owner)
{
	SetScale(_owner->GetScale());
	SetPosition(_spawnPosition);
	m_owner = _owner;
	m_direction = _direction;
	m_speed = _speed;
	m_damage = _damage;
	m_checkCollision = true;
	m_shouldRender = true;
	
	setAngle(vectorToAngle(_direction));
	
}

void Projectile::Update(float _dt) 
{
	move(_dt);
}

void Projectile::OnCollision(Thing* _other)
{
	if (_other != m_owner)
	{

		Damagable* d = dynamic_cast<Damagable*>(_other);
		if (d != nullptr)
		{
			d->TakeDamage(m_damage);
		}
		m_shouldRender = false;
		m_checkCollision = false;
	}
}


