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
	/*addPropertyToProfile(weaponProperties::spreadRandom);
	
	m_SpreadData.spreadRangeDeg.X = -15;
	m_SpreadData.spreadRangeDeg.Y = 15;
	addPropertyToProfile(weaponProperties::burstConstant);
	m_triggerData.shotsFiredperTriggerPull = 2;
	m_burstData.burstShotsVariable.X = 3;
	m_burstData.burstTotalTime = 0.25f;
	m_triggerData.fireRate = 1.0f;
	addPropertyToProfile(weaponProperties::illuminated);
	m_LightData.lightRadius = 1;
	m_LightData.lightIntensity = -5;
	addPropertyToProfile(weaponProperties::bounceExact);
	m_bounceData.bounceMax = 12;*/
		enumLookup.emplace(std::make_pair((int)weaponProperties::spreadRandom,   " Spread Random")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::spreadConstant,  "Spread Constant")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::symetricalSpread, "Symetrical spread")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::piercing,         "Piercing")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::homingClosest,    "homing closest"));
		enumLookup.emplace(std::make_pair((int)weaponProperties::homingLockOnFireDie, "homing LOFD")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::homingLockOnFire, "homing LOF")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::homingLockOnDifferent, "homing Lock on different"));
		enumLookup.emplace(std::make_pair((int)weaponProperties::bounceExact, "Bounce exact"));
		enumLookup.emplace(std::make_pair((int)weaponProperties::bounceVariable, "Bounce Variable"));
		enumLookup.emplace(std::make_pair((int)weaponProperties::areaOfEffectNoDamage, "AOE No damage")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::areaOfEffectDamage, "AOE"));
		enumLookup.emplace(std::make_pair((int)weaponProperties::mineLayer, "Mine Layer")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::everyShotCosts, "Every shot costs")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::illuminated, "illuminated"));   
		enumLookup.emplace(std::make_pair((int)weaponProperties::burstConstant, "Burst constant")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::burstVariable, "Burst Variable")); 
		enumLookup.emplace(std::make_pair((int)weaponProperties::beamWeapon, "Beam weapon")); 
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
				m_triggerData.fireTimer.SetTimer(m_triggerData.fireRate < m_burstData.burstTotalTime ? (m_burstData.burstTotalTime + m_triggerData.fireRate) : m_triggerData.fireRate);
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
			p->OnSpawn(m_holder->GetPosition(), _direction, 3, m_projectileData.DamageAmount, m_weaponProfile, (Thing*)m_holder);
			p->SetStructData(m_world, m_projectileData, m_AOEData, m_LightData, m_bounceData);
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
	return (m_weaponProfile & (Uint32)_property);
}

void Weapon::addPropertyToProfile(weaponProperties _property)
{
	m_weaponProfile |= (Uint32)_property;
}

void Weapon::removePropertyFromProfile(weaponProperties _property)
{
	m_weaponProfile &= ~(Uint32)_property;
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

void Weapon::generateTriggerPullData()
{
	//higher level weapons will tend towards faster fire rates
	int upperLimit = 300 - ((m_weaponLevel * 20) < 200 ? (m_weaponLevel * 5) : 200);
	m_triggerData.fireRate = getRandomInRange(25, upperLimit) * 0.01f;
	m_triggerData.shotsFiredperTriggerPull = floor(2 * m_triggerData.fireRate);
	if (m_triggerData.shotsFiredperTriggerPull < 1)
		m_triggerData.shotsFiredperTriggerPull = 1;
}

void Weapon::generateProjectileData()
{
	m_projectileData.projectileSpeedMultiplier = 1;
	int shotPerPull = (m_triggerData.shotsFiredperTriggerPull * m_burstData.burstShotsVariable.X);
	m_projectileData.DamageAmount = getRandomInRange(5, m_weaponLevel*5) / 2;
	m_projectileData.piercingCount = getChance(75) ? 1 : getRandomInRange(1, (m_weaponLevel / 2) + 1);
	if (m_projectileData.piercingCount > 1)
		addPropertyToProfile(weaponProperties::piercing);
}

void Weapon::generateSpreadData()
{
	if (getChance(50))
	{
		if (getChance(75) || m_triggerData.shotsFiredperTriggerPull < 2)
		{
			addPropertyToProfile(weaponProperties::spreadRandom);

		}
		else
		{
			addPropertyToProfile(weaponProperties::spreadConstant);
			if (getChance(50))//symetrical
			{
				bool around360 = getChance(50);
				addPropertyToProfile(weaponProperties::symetricalSpread);
				for (size_t i = 0; i < m_triggerData.shotsFiredperTriggerPull / 2; i++)
				{
					if (around360)
						m_SpreadData.constSpreadInDegreesPerProjectile.push_back(rand() % 360);
					else
						m_SpreadData.constSpreadInDegreesPerProjectile.push_back(rand() % 180);
				}
				for (size_t i = 0; i < m_triggerData.shotsFiredperTriggerPull / 2; i++)
				{
					m_SpreadData.constSpreadInDegreesPerProjectile.push_back(m_SpreadData.constSpreadInDegreesPerProjectile[i] * -1);
				}
				if (m_triggerData.shotsFiredperTriggerPull % 2 != 0)
				{
					m_SpreadData.constSpreadInDegreesPerProjectile.push_back(0);
				}

			}
			else
			{
				for (size_t i = 0; i < m_triggerData.shotsFiredperTriggerPull; i++)
				{
					m_SpreadData.constSpreadInDegreesPerProjectile.push_back(rand() % 360);
				}
			}
		}
	}
}

void Weapon::generateBurstData()
{
	//higher fire rates reduce chance to add burst property higher weapon levels are more likely to have burst chances
	if (getChance(40 - m_triggerData.fireRate * 7  + m_weaponLevel))
	{
		int upperLimit = (m_weaponLevel < 4 ? 4 : m_weaponLevel);
		if (getChance(50))
		{
			addPropertyToProfile(weaponProperties::burstConstant);
			m_burstData.burstShotsVariable.X = getRandomInRange(1, upperLimit);
		}
		else
		{
			addPropertyToProfile(weaponProperties::burstVariable);
			m_burstData.burstShotsVariable.X = getRandomInRange(1, upperLimit);
			m_burstData.burstShotsVariable.Y = getRandomInRange(m_burstData.burstShotsVariable.X + 2, m_burstData.burstShotsVariable.X + upperLimit);
		}
		m_burstData.burstTotalTime = getRandomInRange(5, 50) * 0.01f;
	}
}

void Weapon::generateHomingData()
{
	if (getChance(5 + m_weaponLevel * 5))
	{
		int chance = rand() % 100;
		if (chance > 80)
		{
			addPropertyToProfile(weaponProperties::homingLockOnDifferent);
		}
		else if (chance > 60)
		{
			addPropertyToProfile(weaponProperties::homingClosest);
		}
		else
		{
			if (getChance(50))
			{
				addPropertyToProfile(weaponProperties::homingLockOnFireDie);
			}
			else
			{
				addPropertyToProfile(weaponProperties::homingLockOnFire);
			}
		}
	}
}

void Weapon::generateAOEData()
{
	if (getChance(15))
	{
		if (getChance(75))
		{
			addPropertyToProfile(weaponProperties::areaOfEffectDamage);
		}
		else
		{
			addPropertyToProfile(weaponProperties::areaOfEffectNoDamage);
		}
		m_AOEData.AoeRadiusInTiles = getRandomInRange(1, m_weaponLevel);
	}
}

void Weapon::generateBounceData()
{
	if (getChance(15))
	{
		if (getChance(40))
		{
			addPropertyToProfile(weaponProperties::bounceExact);
		}
		else
		{
			addPropertyToProfile(weaponProperties::bounceVariable);
		}
	}
	m_bounceData.bounceMax = getRandomInRange(1, 1 + m_weaponLevel);
}

bool Weapon::getChance(int _percentChance)
{
	return (rand() % 100) < _percentChance;
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
	return propertyInProfile(weaponProperties::illuminated);
}

void Weapon::GenerateWeapon(int _weaponLevel)
{
	m_weaponLevel = _weaponLevel;
	if (!m_randomWeapon)
	{
		generateShotgun();
	}
	else
	{
		if (getChance(m_weaponLevel * 3))
		{
			addPropertyToProfile(weaponProperties::everyShotCosts);
		}
		if (getChance(30))
		{
			addPropertyToProfile(weaponProperties::illuminated);
			m_LightData.lightRadius = getRandomInRange(1, 3);
			m_LightData.lightIntensity = m_projectileData.DamageAmount * 1.5f;
		}
		generateTriggerPullData();
		generateBurstData();
		generateProjectileData();
		generateSpreadData();
		generateHomingData();
		generateAOEData();
		/*generateBounceData();*/
	}
	


}

void Weapon::PrintWeaponInfo()
{
	for (auto i = enumLookup.begin(); i != enumLookup.end(); i++)
	{
		if (propertyInProfile((weaponProperties)(*i).first))
			printf("Weapon has %s property\n", (*i).second.c_str());
	}
	m_triggerData.PrintData();
	m_projectileData.PrintData();
	if (propertyInProfile(weaponProperties::spreadConstant) || propertyInProfile(weaponProperties::spreadRandom))
	{
		m_SpreadData.PrintData();
	}
	if (propertyInProfile(weaponProperties::burstConstant) || propertyInProfile(weaponProperties::burstVariable))
	{
		m_burstData.PrintData();
	}
	if (propertyInProfile(weaponProperties::illuminated))
	{
		
		m_LightData.PrintData();
	}
	if (propertyInProfile(weaponProperties::areaOfEffectDamage) || propertyInProfile(weaponProperties::areaOfEffectNoDamage))
	{
		m_AOEData.PrintData();
	}
	if (propertyInProfile(weaponProperties::bounceExact) || propertyInProfile(weaponProperties::bounceVariable))
	{
		m_bounceData.PrintData();
	}
}




void Weapon::generatePistol()
{

	//higher level weapons will tend towards faster fire rates
	
	m_triggerData.fireRate = getRandomInRange(0.75f, 1.0f);
	m_triggerData.shotsFiredperTriggerPull = 1;

	m_projectileData.projectileSpeedMultiplier = 1;
	m_projectileData.DamageAmount = 1;
	addPropertyToProfile(weaponProperties::spreadRandom);
	m_SpreadData.spreadRangeDeg.Set(-5, 5);


}

void Weapon::generateShotgun()
{

	m_triggerData.fireRate = getRandomInRange(1.0f, 1.5f);
	m_triggerData.shotsFiredperTriggerPull = 2;
	m_projectileData.projectileSpeedMultiplier = 1;
	m_projectileData.DamageAmount = 1;
	addPropertyToProfile(weaponProperties::spreadRandom);
	m_SpreadData.spreadRangeDeg.Set(-15, 15);
	addPropertyToProfile(weaponProperties::burstConstant);
	m_burstData.burstShotsVariable.X = 3;
	addPropertyToProfile(weaponProperties::everyShotCosts);
	m_burstData.burstTotalTime = 0.1f;

}

void Weapon::generateMachinegun()
{
}

void Weapon::generateSniperRifle()
{
}

void Weapon::generateTriShot()
{
}

void Weapon::generateHexShot()
{
}

void Weapon::generateAutoPistol()
{
}

void Weapon::generateAutoShotgun()
{
}

void Weapon::generateAutoMachinegun()
{
}

void Weapon::generateMinigun()
{
}

void Weapon::generateMissileLauncher()
{
}

void Weapon::generateAutoHexShot()
{
}

void Weapon::generateAutoTriShot()
{
}

void Weapon::generateAutoMinigun()
{
}

void Weapon::generateAutoMissileLauncher()
{
}

void Weapon::generateAutoSniperRifle()
{
}