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
	Weapon();
protected:
	float m_fireRate = 1.0f;
	Timer m_fireTimer = Timer(1.0f);
	int m_ammo = 100;
	int m_ammoMax = 100;
	Player* m_holder;
	void drop();
	Projectile* getProjectile();
	std::vector<Projectile*> m_projectiles;
public:
	Weapon(Scene* _scene);
	~Weapon();
	bool AddAmmo(int _amount);
	void SetHolder(Player* _holder);
	void Fire(Vector2 _direction, bool _costAmmo = true);
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
	void Update(float _dt) override;
	int GetAmmo();
	float GetAmmoAsPercent();
};


