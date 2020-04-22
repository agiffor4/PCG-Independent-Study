#pragma once
#include "Interactable.h"
#include <vector>
#include <map>
#include "Timer.h"
#include "WeaponStructs.h"
class Player;
class Projectile;
class Scene;
class World;

class Weapon :
	public Interactable
{
friend Projectile;
private:
	Scene* m_scene = nullptr;
	World* m_world;
public:
	enum class weaponProperties {
		spreadRandom = 1, //weapon has a variable spread per trigger pull
		spreadConstant = 2, //weapon has a consistent spread per trigger pull 
		symetricalSpread = 4, //no mechanical effect but flagged so it can be known for naming reasons
		piercing = 8, //projectile goes through multipule enemies before dieing
		homingClosest = 16, //homes to nearest target, target is always closest enemy
		homingLockOnFireDie = 32, //picks target to home to on fire, continues to target's last location if target dies, and then te projectile dies
		homingLockOnFire = 64, //picks target to home to on fire
		homingLockOnDifferent = 128, //on fire each missile locks onto a different target, repeating lockon for each fired projectile only if there are no new targets, projectiles will not pick new targets if their original target dies
		bounceExact = 256, //bounces at at an exact angle if it hits a wall
		bounceVariable = 512, //bounces at an angle modifed by a variable amount on hitting a wall
		areaOfEffectNoDamage = 1024, //aoe that does not damage originator
		areaOfEffectDamage = 2048, //aoe that does damage originator		
/*++*/	mineLayer = 4096, //fires mines instead of projectiles 
		everyShotCosts = 8192, //in cases where the weapon fires multipule projectiles in a trigger pull does every shot drain the ammo amount or only one
		illuminated = 16384, //provideslightsource
		burstConstant = 32768, //+if a weapon is burst it fires x number of projectiles per trigger pull with a delay between each shot
		burstVariable = 65536, //+if a weapon is burst variable it fires x to y number of projectiles per trigger pull with a delay between each shot
/*++*/	beamWeapon = 131072 //
	};
	std::map<int, std::string> enumLookup;
protected:


	Player* m_holder;
	
	//spread variables
	WeaponStructs::SpreadStruct m_SpreadData = WeaponStructs::SpreadStruct();
	//burst variables
	WeaponStructs::BurstStruct m_burstData = WeaponStructs::BurstStruct();
	//Light variables
	WeaponStructs::LightStruct m_LightData = WeaponStructs::LightStruct();
	//ProjectileData
	WeaponStructs::ProjectileStruct m_projectileData = WeaponStructs::ProjectileStruct();
	WeaponStructs::BounceStruct m_bounceData = WeaponStructs::BounceStruct();

	//piercing variables
	int m_pierceCount = 1;
	Uint32 m_weaponProfile = 0;

	//Ammo variables
	int m_ammo = 100;
	int m_ammoMax = 100;
	int m_ammoGainFromPickup = 5;
	int m_ammoCostPerTriggerPull = 5;

	//Trigger pull variables
	WeaponStructs::TriggerPullStruct m_triggerData = WeaponStructs::TriggerPullStruct();
	//AOE damage
	WeaponStructs::AOEStruct m_AOEData = WeaponStructs::AOEStruct();
	
	int m_weaponLevel = 1;
	bool m_randomWeapon = false;
	Projectile* getProjectile();
	std::vector<Projectile*> m_projectiles;
	void fire(Vector2 _direction, bool _costAmmo = true, bool _ignoreCooldown = false);
	
	void spreadFireCode(Vector2 _direction, int i);
	void burstFireCode(float _dt);
	void applyHomingAsApplicable();
	void setProjectileHomeTargetsToNearest(float _maxDistance);

	//UTILITY
	int getRandomInRange(int _min, int _max);
	bool propertyInProfile(weaponProperties _property);
	void addPropertyToProfile(weaponProperties _property);
	void removePropertyFromProfile(weaponProperties _property);
	Vector2 rotateDirectionByDegrees(Vector2 _direction, float _degrees);
	
	void generateTriggerPullData();
	void generateProjectileData();
	void generateSpreadData();
	void generateBurstData();
	void generateHomingData();
	void generateAOEData();
	void generateBounceData();
	bool getChance(int _percentChance);

	//tier 1
	void generatePistol();
	//tier 2
	
	void generateShotgun();
	void generateMachinegun();
	//tier 3
	void generateSniperRifle();
	void generateTriShot();
	void generateHexShot();
	void generateAutoPistol();
	//tier 4
	void generateAutoShotgun();
	void generateAutoMachinegun();
	void generateMinigun();
	void generateMissileLauncher();
	//tier 5
	void generateAutoMissileLauncher();
	void generateAutoSniperRifle();
	Enums::DamageType m_damageType = Enums::DamageType::Piercing;
	void generateWeaponOfType(int _weapon);
	void generateWeaponOfType(const std::vector<int>& _weapon);
public:
	Weapon();
	~Weapon();
	void InitializeWeapon(Scene* _scene, World* _world);
	bool AddAmmo(int _amount);
	void SetHolder(Player* _holder);
	virtual void Fire(Vector2 _direction);
	bool Interaction(Thing* _thingInitatingInteraction) override;
	void InteractionOnEnteringTile(Thing* _thingInitatingInteraction) override;
	void Update(float _dt) override;
	int GetAmmo();
	float GetAmmoAsPercent();
	bool ShouldSetLOS();
	void GenerateWeapon(int _weaponLevel);
	
	void PrintWeaponInfo();
};


