#include "Weapon.h"
#include "Projectile.h"
#include "Player.h"
#include "Scene.h"
#include  "World.h"
#include "Enemy.h"
Projectile* Weapon::getProjectile()
{
	
	for (size_t i = 0; i < m_projectiles.size(); i++)
	{
		if (!m_projectiles[i]->ShouldCheckCollision())
		{
			return m_projectiles[i];		
		}
	}
	m_projectiles.push_back(new Projectile());
	m_projectiles[m_projectiles.size() - 1]->Init("img/Projectile.png", "Projectile", m_rendererRef);
	m_scene->AddRenderable(m_projectiles[m_projectiles.size() - 1]);
	return m_projectiles[m_projectiles.size() -1];
}

Weapon::Weapon()
{
	//shotgun
	addPropertyToProfile(weaponProperties::spreadRandom);
	
	m_SpreadData.spreadRangeDeg.X = -15;
	m_SpreadData.spreadRangeDeg.Y = 15;
	/*addPropertyToProfile(weaponProperties::burstConstant);
	m_triggerData.shotsFiredperTriggerPull = 2;
	m_burstData.burstShotsVariable.X = 3;
	m_burstData.burstTotalTime = 0.25f;
	m_triggerData.fireRate = 1.0f;*/
	addPropertyToProfile(weaponProperties::illuminated);
	m_LightData.lightRadius = 1;
}

Weapon::~Weapon()
{
}

void Weapon::InitializeWeapon(Scene* _scene, World* _world)
{
	m_scene = _scene;
	m_world = _world;
	m_rendererRef = _scene->GetRenderer();
	
}

bool Weapon::AddAmmo(int _amount)
{
	if (m_ammo == m_ammoMax)
		return false;
	m_ammo += _amount * m_ammoGainFromPickup;
	if (m_ammo > m_ammoMax)
		m_ammo = m_ammoMax;
	return true;
}

void Weapon::SetHolder(Player* _holder)
{
	m_holder = _holder;
}
void Weapon::Fire(Vector2 _direction) {
	
	if (!m_triggerData.fireTimer.GetShouldCountDown())
	{
		if (propertyInProfile(weaponProperties::burstConstant) || propertyInProfile(weaponProperties::burstVariable))
		{
			
			if (!m_burstData.burstFireing)
			{
				m_burstData.burstFireing = true;
				
				if (propertyInProfile(weaponProperties::burstConstant))
				{
					m_burstData.burstShotsForCurrentBurst = m_burstData.burstShotsVariable.X; //sets the number shots for this trigger pull to the consistent value
				}
				else
				{
					m_burstData.burstShotsForCurrentBurst = getRandomInRange(m_burstData.burstShotsVariable.X, m_burstData.burstShotsVariable.Y); //sets the number of shot for this trigger pull to a number in the range
				}
				m_burstData.burstShotsRemaining = m_burstData.burstShotsForCurrentBurst;
				m_burstData.burstTimer.SetTimer(m_burstData.burstTotalTime / (float)m_burstData.burstShotsRemaining);
				m_triggerData.fireTimer.SetTimer(m_burstData.burstTotalTime + m_triggerData.fireRate);
			}
			
		}
		else
		{
			for (size_t i = 0; i < m_triggerData.shotsFiredperTriggerPull; i++)// fire shot for each trigger pull
			{
				spreadFireCode(_direction, i);
			}
		}
		
	}
	
	
	
}


void Weapon::spreadFireCode(Vector2 _direction, int i)
{
	float degrees = 0;
	if (propertyInProfile(weaponProperties::spreadRandom)) //if spread is random get that spread
	{
		degrees = getRandomInRange(m_SpreadData.spreadRangeDeg.X, m_SpreadData.spreadRangeDeg.Y);
		_direction = rotateDirectionByDegrees(_direction, degrees);
	}
	else if (propertyInProfile(weaponProperties::spreadConstant)) //if spread is random get that spread
	{

		if (i < m_SpreadData.constSpreadInDegreesPerProjectile.size())
			degrees = m_SpreadData.constSpreadInDegreesPerProjectile[i];
		_direction = rotateDirectionByDegrees(_direction, degrees);
	}
	fire(_direction, propertyInProfile(weaponProperties::everyShotCosts) || i == 0, true);
}
void Weapon::burstFireCode(float _dt)
{
	if (m_burstData.burstFireing)
	{
		if (m_burstData.burstTimer.CountDown(_dt))
		{
			for (size_t i = 0; i < m_triggerData.shotsFiredperTriggerPull; i++)// fire shot for each trigger pull
			{
				spreadFireCode(m_holder->GetAimDirection(), m_burstData.burstShotsForCurrentBurst - m_burstData.burstShotsRemaining);
			}
			m_burstData.burstShotsRemaining--;
			if (m_burstData.burstShotsRemaining < 1)
			{
				m_burstData.burstFireing = false;
			}
		}
		
	}
}
void Weapon::applyHomingAsApplicable()
{

	int maxDistance = 256;
	setProjectileHomeTargetsToNearest(maxDistance);
	if (propertyInProfile(weaponProperties::homingLockOnFireDie))
	{
		for (size_t i = 0; i < m_projectiles.size(); i++)
		{
			if (m_projectiles[i]->ShouldCheckCollision())
			{
				Enemy* closest = m_world->GetNearestEnemy(m_holder->GetPosition(), maxDistance);
				if (closest != nullptr)
					m_projectiles[i]->SetHomingTarget(closest);
			}
		}
	}
	else if (propertyInProfile(weaponProperties::homingLockOnDifferent))
	{
		std::vector<Enemy*> inRadius = m_world->GetEnemiesInRadius(m_holder->GetPosition(), 256);

	}
}
void Weapon::setProjectileHomeTargetsToNearest(float _maxDistance)
{
	if (propertyInProfile(weaponProperties::homingClosest) || propertyInProfile(weaponProperties::homingLockOnFire))
	{
		for (size_t i = 0; i < m_projectiles.size(); i++)
		{
			if (m_projectiles[i]->ShouldCheckCollision())
			{
				if (!m_projectiles[i]->HomingTargetIsValid() || propertyInProfile(weaponProperties::homingClosest))
				{
					Enemy* closest = m_world->GetNearestEnemy(m_holder->GetPosition(), _maxDistance);
					if (closest != nullptr)
						m_projectiles[i]->SetHomingTarget(closest);
				}
			}
		}
	}
	
}
void Weapon::fire(Vector2 _direction, bool _costAmmo, bool _ignoreCooldown)
{
	if (!m_triggerData.fireTimer.GetShouldCountDown() || _ignoreCooldown)
	{
		if (m_ammo > 0 || !_costAmmo)
		{
			m_triggerData.fireTimer.SetShouldCountDown(true);
			Projectile* p = getProjectile();
			p->OnSpawn(m_holder->GetPosition(), _direction, 3, 5, m_weaponProfile, (Thing*)m_holder);
			p->SetStructData(m_world, m_projectileData, m_AOEData, m_LightData);
			if (_costAmmo)
				m_ammo--;
		}

	}
	
}

int Weapon::getRandomInRange(int _min, int _max)
{
	if (_max < _min)
	{
		printf("_min out of bounds! returning -1.\n");
		return -1;
	}
	return (rand() % (_max + 1 - _min)) + _min;
}

bool Weapon::propertyInProfile(weaponProperties _property)
{
	return (m_weaponProfile & (int)_property);
}

void Weapon::addPropertyToProfile(weaponProperties _property)
{
	m_weaponProfile |= (int)_property;
}

void Weapon::removePropertyFromProfile(weaponProperties _property)
{
	m_weaponProfile ^= (int)_property;
}

Vector2 Weapon::rotateDirectionByDegrees(Vector2 _direction, float _degrees)
{
	while(_degrees < 0)
		_degrees += 360;
	while(_degrees > 360)
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



bool Weapon::Interaction(Thing* _thingInitatingInteraction)
{
	/*Player* p = dynamic_cast<Player*>(_thingInitatingInteraction);
	p->EquipWeapon(this);*/
	return true;
	
}

void Weapon::InteractionOnEnteringTile(Thing* _thingInitatingInteraction)
{
}



void Weapon::Update(float _dt)
{
	m_triggerData.fireTimer.CountDownAutoCheckBool(_dt);
	if (m_burstData.burstFireing)
	{
		burstFireCode(_dt);
	}
	if (propertyInProfile(weaponProperties::homingClosest))
	{
		setProjectileHomeTargetsToNearest(256);
	}
}

int Weapon::GetAmmo()
{
	return m_ammo;
}

float Weapon::GetAmmoAsPercent()
{
	return m_ammo / m_ammoMax;
}

bool Weapon::ShouldSetLOS()
{
	return propertyInProfile(weaponProperties::illuminated) && m_triggerData.fireTimer.GetShouldCountDown();
}

void Weapon::GenerateWeapon()
{
}
