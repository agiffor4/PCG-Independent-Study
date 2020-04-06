#include "Projectile.h"
#include "Damagable.h"
#include "Tile.h"
#include "Enemy.h"
#include "World.h"
#define Property Weapon::weaponProperties
void Projectile::move(float _dt)
{
	if (m_checkCollision)
	{

		Vector2 pos;
		float speed = m_speed * m_speedMultiplier * _dt * m_projectileData.projectileSpeedMultiplier;
		if (m_homingTarget != nullptr)
		{
			pos = GetPosition() + ((GetPosition() - m_homingTarget->GetPosition()).Normalized() * speed);
			if (!HomingTargetIsValid() && Vector2::GetDistanceLessThan(pos, m_homingTarget->GetPosition(), 16))
			{
				die();
			}
		}
		else
		{
			pos = GetPosition() + (m_direction *speed);
		}
		SetPosition(pos);
		if (Vector2::GetDistanceGreaterThan(GetPosition(), m_currentTileIn->GetPosition(), m_currentTileIn->GetDestination().w*0.5f))
		{
			
			if (hasProperty(Property::illuminated))
				Illuminate(false);
			m_currentTileIn = m_world->GetNeighborNearestPoint(GetPosition(), m_currentTileIn->GetPositionInVector(), true, true);
			if (hasProperty(Property::illuminated))
				Illuminate(true);
		}
	}
}



bool Projectile::hasProperty(Weapon::weaponProperties _property)
{
	return m_weaponProperties & (int)_property;
}

void Projectile::die()
{
	if (hasProperty(Property::areaOfEffectDamage) || hasProperty(Property::areaOfEffectNoDamage))
	{
		if (m_currentTileIn != nullptr)
		{
			std::vector<Enemy*> enemiesInRange = m_world->GetEnemiesInTileRadius(m_currentTileIn->GetPositionInVector(), m_aoeData.AoeRadiusInTiles);	
			for (size_t i = 0; i < enemiesInRange.size(); i++)
			{
				enemiesInRange[i]->TakeDamage(m_damage * m_aoeData.AoePercentageFallOffPerUnitDistance);
			}
			if (hasProperty(Property::areaOfEffectDamage))
			{
				if (Vector2::GetDistanceLessThan(m_currentTileIn->GetPositionInGrid(), m_owner->GetLocation()->GetPositionInGrid(), m_aoeData.))
				{
					Damagable* d = dynamic_cast<Damagable*>(m_owner);
					if (d != nullptr)
					{
						d->TakeDamage(m_damage * m_aoeData.AoePercentageFallOffPerUnitDistance);
					}
				}
				
			}
		}
		
	}
	if (hasProperty(Property::illuminated))
	{
		Illuminate(false);
	}
	m_shouldRender = false;
	m_checkCollision = false;
}

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::OnSpawn(const Vector2& _spawnPosition, const Vector2& _direction, float _speed, int _damage, Uint32 _weaponProfile, Thing* _owner)
{
	
	m_weaponProperties = _weaponProfile;
	SetScale(_owner->GetScale());
	SetPosition(_spawnPosition);
	m_owner = _owner;
	m_direction = _direction;
	m_speed = _speed;
	m_damage = _damage;
	m_checkCollision = true;
	m_shouldRender = true;
	m_currentTileIn = m_owner->GetLocation();
	setAngle(vectorToAngle(_direction));
	
}

void Projectile::SetStructData(World* _world, WeaponStructs::ProjectileStruct _projectileData, WeaponStructs::AOEStruct _aoeData, WeaponStructs::LightStruct _lightData)
{
	m_world = _world;
	m_projectileData = _projectileData;
	m_aoeData = _aoeData;
	m_lightData = _lightData;
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
		die();
	}
}


void Projectile::SetHomingTarget(Enemy* _toSet)
{
	m_homingTarget = _toSet;
}

bool Projectile::HomingTargetIsValid()
{
	return (m_homingTarget != nullptr && !m_homingTarget->IsDead());
}

//Illumination

std::set<Tile*> Projectile::GetEffectedTiles(Tile* _epicenter)
{
	std::set<Tile*> toIlluminate;
	int center = _epicenter->GetPositionInVector();

	std::vector<Tile*> neigbors = m_world->GetNeighbors(center);
	for (size_t i = 0; i < neigbors.size(); i++)
	{
		toIlluminate.emplace(neigbors[i]);
	}
	std::vector<Tile*> tempNeigbors;
	for (size_t k = 0; k < m_lightData.lightRadius; k++)
	{

		for (size_t i = 0; i < tempNeigbors.size(); i++)
			toIlluminate.emplace(tempNeigbors[i]);
		tempNeigbors.clear();
		for (auto itt = toIlluminate.begin(); itt != toIlluminate.end(); itt++)
		{
			std::vector<Tile*> templist = m_world->GetNeighbors((*itt)->GetPositionInVector());
			for (size_t i = 0; i < templist.size(); i++)
			{
				if (templist[i]->IsPassable())
				{
					tempNeigbors.push_back(templist[i]);
				}

			}

		}
	}
	toIlluminate.emplace(_epicenter);

	return toIlluminate;
}


void Projectile::Illuminate(bool _lightUp) {
	if (m_world != nullptr)
	{
		Tile* epicenter = m_currentTileIn;
		if (epicenter != nullptr)
		{
			std::set<Tile*> inLight = GetEffectedTiles(epicenter);
			for (auto i = inLight.begin(); i != inLight.end(); i++)
			{
				int dist = (_lightUp ? Vector2::GetMagnitude(m_currentTileIn->GetPositionInGrid(), (*i)->GetPositionInGrid()) : -1);
				(*i)->SetIlluminated(_lightUp, dist, m_lightData.lightIntensity);
				(*i)->SetFogOfWar(!_lightUp, dist);
			}
		}
	}
}
/*
void Projectile::SetLocation(Tile* _newLocation)
{

	Illuminate(false);
	Thing::SetLocation(_newLocation);
	Illuminate(true);
}
*/


