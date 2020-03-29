#include "Weapon.h"
#include "Projectile.h"
#include "Player.h"
#include "Scene.h"
void Weapon::drop()
{
}

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
	m_projectiles[m_projectiles.size() - 1]->Init("img/Projectile.bmp", "Projectile", m_rendererRef);
	m_scene->AddRenderable(m_projectiles[m_projectiles.size() - 1]);
	return m_projectiles[m_projectiles.size() -1];
}

Weapon::Weapon(Scene* _scene)
{
	m_scene = _scene;
	m_rendererRef = _scene->GetRenderer();
}

Weapon::~Weapon()
{/*
	for (size_t i = 0; i < m_projectiles.size(); i++)
	{
		m_scene->RemoveRenderable(m_projectiles[i]);
	}
	m_projectiles.clear();*/
}

bool Weapon::AddAmmo(int _amount)
{
	if (m_ammo == m_ammoMax)
		return false;
	m_ammo += _amount;
	if (m_ammo > m_ammoMax)
		m_ammo = m_ammoMax;
	return true;
}

void Weapon::SetHolder(Player* _holder)
{
	m_holder = _holder;
}

void Weapon::Fire(Vector2 _direction, bool _costAmmo)
{
	if (!m_fireTimer.GetShouldCountDown())
	{
		if (m_ammo > 0 || !_costAmmo)
		{
			m_fireTimer.SetShouldCountDown(true);
			Projectile* p = getProjectile();
			p->OnSpawn(m_holder->GetPosition(), _direction, 3, 5, (Thing*)m_holder);
			if (_costAmmo)
				m_ammo--;
		}

	}
	
}

bool Weapon::Interaction(Thing* _thingInitatingInteraction)
{
	Player* p = dynamic_cast<Player*>(_thingInitatingInteraction);
	p->EquipWeapon(this);
	return true;
	
}

void Weapon::InteractionOnEnteringTile(Thing* _thingInitatingInteraction)
{
}



void Weapon::Update(float _dt)
{
	m_fireTimer.CountDownAutoCheckBool(_dt);
}
