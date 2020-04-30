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
		if (m_currentTileIn != nullptr && Vector2::GetDistanceGreaterThan(GetPosition(), m_currentTileIn->GetPosition(), m_currentTileIn->GetDestination().w*0.5f))
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
				enemiesInRange[i]->TakeDamage(m_damage * m_aoeData.AoePercentageFallOffPerUnitDistance, m_projectileData.KindOfDamage);
			}
			if (hasProperty(Property::areaOfEffectDamage))
			{
				if (Vector2::GetDistanceLessThan(m_currentTileIn->GetPositionInGrid(), m_owner->GetLocation()->GetPositionInGrid(), m_aoeData.AoeRadiusInTiles))
				{
					Damagable* d = dynamic_cast<Damagable*>(m_owner);
					if (d != nullptr)
					{
						d->TakeDamage(m_damage * m_aoeData.AoePercentageFallOffPerUnitDistance, m_projectileData.KindOfDamage);
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
	m_bounceData.ClearLastThingHit();
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

void Projectile::SetStructData(World* _world, WeaponStructs::ProjectileStruct _projectileData, WeaponStructs::AOEStruct _aoeData, WeaponStructs::LightStruct _lightData, WeaponStructs::BounceStruct _bounceData)
{
	m_world = _world;
	m_projectileData = _projectileData;
	m_aoeData = _aoeData;
	m_lightData = _lightData;
	m_bounceData = _bounceData;

}

void Projectile::Update(float _dt) 
{
	move(_dt);
	if (hasProperty(Property::bounceExact) || hasProperty(Property::bounceVariable))
	{
		m_bounceData.bounceCoolDown.CountDownAutoCheckBool(_dt);
	}
}

void Projectile::OnCollision(Thing* _other)
{
	if (_other != m_owner)
	{

		Damagable* d = dynamic_cast<Damagable*>(_other);
		if (hasProperty(Property::piercing))
		{
			if (m_projectileData.LastThingHit != _other)
			{
				m_projectileData.piercingCount--;
				if (m_projectileData.piercingCount < 1)
				{
					die();
				}
				if (d != nullptr)
					d->TakeDamage(m_damage, m_projectileData.KindOfDamage);
				else
					die();
				m_projectileData.LastThingHit = _other;
			}
			
		}
		else if (hasProperty(Property::bounceExact) || hasProperty(Property::bounceVariable))
		{
			
				if (m_bounceData.bounceMax > 0 && !m_bounceData.RecentlyHit(_other))
				{
					bool result = !m_bounceData.bounceCoolDown.GetShouldCountDown();
					if (result)
					{

						Vector2 right = rotateDirectionByDegrees(m_direction, 90);
						Vector2 OtherPos = _other->GetPosition();
						float dot = Vector2::Dot(OtherPos.Normalized(), right.Normalized());
						int degrees = 0;
						printf("dot is %f\n", dot);
						if (abs(dot) < 0.6f) //to left or right
						{
							m_direction.X *= -1;
						/*	if (dot > 0)
							{
								if (m_direction.Y > 0)
								{
									degrees = 90;
								}
								else
								{
									degrees = -90;
								}
							}
							else
							{
								if (m_direction.Y < 0)
								{
									degrees = 90;
								}
								else
								{
									degrees = -90;
								}
							}*/
						}
						else //above or below
						{
							/*if (dot > 0)
							{
								if (m_direction.X > 0)
								{
									degrees = 90;
								}
								else
								{
									degrees = -90;
								}
							}
							else
							{
								if (m_direction.X < 0)
								{
									degrees = 90;
								}
								else
								{
									degrees = -90;
								}
							}*/
							m_direction.Y *= -1;
						}
						
						if (hasProperty(Property::bounceVariable))
						{
							degrees += ((rand() % 30) - 15);
						}
						m_direction = rotateDirectionByDegrees(m_direction, degrees);
						setAngle(vectorToAngle(m_direction));
						m_bounceData.bounceCoolDown.SetShouldCountDown(true);
					}
					
				}
				else
				{
					die();
				}
				if (!m_bounceData.RecentlyHit(_other))
					m_bounceData.bounceMax--;
				m_bounceData.thisBounce++;
				m_bounceData.LastThingHit[m_bounceData.Index()] = _other;
			
		}
		else
		{

			if (d != nullptr)
				d->TakeDamage(m_damage, m_projectileData.KindOfDamage);
			die();
		}

		

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
				if (templist[i]->IsIlluminatable())
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

void Projectile::Render(SDL_Renderer* _renderer)
{
	if (m_shouldRender)
	{
		if (m_currentTileIn != nullptr)
		{
			storeTextureColorMod();
			setTextureColorMod(m_currentTileIn->GetIllumination());
		}
		
		Renderable::Render(_renderer);
		if (m_currentTileIn != nullptr)
			revertTextureColorMod();
	}
	
}


Vector2 Projectile::rotateDirectionByDegrees(Vector2 _direction, float _degrees)
{
	while (_degrees < 0)
		_degrees += 360;
	while (_degrees > 360)
		_degrees -= 360;
	double theta = _degrees * deg2Rad;
	double cs = cos(theta);
	double sn = sin(theta);
	float x = _direction.X * cs - _direction.Y * sn;
	float y = _direction.X * sn + _direction.Y * cs;
	_direction.X = x;
	_direction.Y = y;
	return _direction;
}

void Projectile::getExactBounceDirection(int& _degreeOffset, const Vector2& _surfacePosition)
{
	float currentDistance = Vector2::GetMagnitude(GetPosition(), _surfacePosition);
	Vector2 direction1 = rotateDirectionByDegrees(m_direction, _degreeOffset);
	Vector2 aproxNextPos1 = GetPosition() + (direction1 * m_speed * m_speedMultiplier * 0.0166667f * m_projectileData.projectileSpeedMultiplier);
	//Vector2 direction2 = rotateDirectionByDegrees(m_direction, _degreeOffset * -1);
	//Vector2 aproxNextPos2 = GetPosition() + (direction2 * m_speed * m_speedMultiplier * 0.0166667f * m_projectileData.projectileSpeedMultiplier);
	float distance1 = Vector2::GetMagnitude(aproxNextPos1, _surfacePosition);
	if (distance1 < currentDistance)
	{
		_degreeOffset *= -1;
	}
}

