#pragma once
#include "Interactable.h"
#include <vector>
#include "Timer.h"
class Player;
class Projectile;
class Scene;
class Weapon :
	public Interactable
{
private:
	Scene* m_scene = nullptr;
protected:
	float m_fireRate = 1.0f;
	Timer m_fireTimer = Timer(1.0f);
	int m_ammo = 100;
	int m_ammoMax = 100;
	Player* m_holder;
	
	Projectile* getProjectile();
	std::vector<Projectile*> m_projectiles;
	void fire(Vector2 _direction, bool _costAmmo = true, bool _ignoreCooldown = false);
public:
	Weapon();
	~Weapon();
	void InitializeWeapon(Scene* _scene);
	bool AddAmmo(int _amount);
	void SetHolder(Player* _holder);
	virtual void Fire(Vector2 _direction);
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
	void Update(float _dt) override;
	int GetAmmo();
	float GetAmmoAsPercent();
};


