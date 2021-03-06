#pragma once
#include "WeaponHolder.h"
#include "EnemyCombat.h"
#include "EnemyDataStructs.h"
#include "RoomData.h"
#include "IInputHandler.h"
class TextA;
class Scene;
class World;
class Player;
class Enemy :
	public WeaponHolder, public IInputHandler
{
public:
	enum class EnemyProperty
	{
				combatRanged = 1, //ranged combat exclusive with melee combat
				combatMelee = 2, //melee combat exclusive with ranged combat
				combatSummon = 4, //summons other enemies, not exclusive with other combat properties
				movemetMoves = 8, //if true allows movement, if false stationary
				defenseShieldTimed = 16,
				defenseShieldBreakable = 32,
				defenseLeaveBarricades = 64,
				healthRegen = 128,
				contactPassive = 256,
				visibilityInvisible = 512,
				visibilityFlicker = 1024,
				visibilityVisible = 2048,
				mineLayer = 4096,
				behaviorSeekout = 8192,
				behaviorKeepDistance = 16384,
				behaviorCharge = 32768,
				behaviorPatrol = 65536,
/*+18+*/		defenseDodge = 131072,
/*+19+*/		// = 262144,
/*+20+*/		// = 524288
		

	};
protected:
	enum class Direction
	{
		NONE = -1,
		UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT
	};
	TextA* m_nameDisplayer = nullptr;
	TextA* m_HealthDisplayer = nullptr;
	EnemyCombat m_combat = EnemyCombat();
	float m_moveRate = 1.0f; //in seconds per tile
	float m_attackRate = 1.0f;// in seconds
	Uint32 m_EnemyProfile = 0;
	World* m_world = nullptr;
	std::string m_EnemyName;
	Player* m_player = nullptr;
	Renderable* m_shieldImage = nullptr;
	RoomData m_roomSpawnedIn = RoomData();
	//UTILITY
	int getRandomInRange(int _min, int _max);
	float getRandomInRange(float _min, float _max);
	float getRandomInRange(double _min, double _max);
	bool propertyInProfile(EnemyProperty _property);
	void addPropertyToProfile(EnemyProperty _property);
	void removePropertyFromProfile(EnemyProperty _property);
	Vector2 rotateDirectionByDegrees(Vector2 _direction, float _degrees);
	bool getChance(int _percentChance);
	bool moveToPositionIfFree(Tile* _t);
	Direction IsInLine1RelativeTo2(int _point1, int _point2, int worldWdith = -1);

	//data variables
	EnemyDataStructs::MovementStruct m_movementData = EnemyDataStructs::MovementStruct();
	EnemyDataStructs::ChargeStruct m_chargeData = EnemyDataStructs::ChargeStruct();
	EnemyDataStructs::ShieldStruct m_shieldData = EnemyDataStructs::ShieldStruct();
	EnemyDataStructs::VisibilityStruct m_visibilityData = EnemyDataStructs::VisibilityStruct();
	EnemyDataStructs::BehaviorStruct m_behaviorData = EnemyDataStructs::BehaviorStruct();
	EnemyDataStructs::ContactStruct	m_contactData = EnemyDataStructs::ContactStruct();
	EnemyDataStructs::MineLayerStruct m_mineLayerData = EnemyDataStructs::MineLayerStruct();
	EnemyDataStructs::DodgeStruct m_dodgeData = EnemyDataStructs::DodgeStruct();
	EnemyDataStructs::BarrierStruct	m_barrierData = EnemyDataStructs::BarrierStruct();
	EnemyDataStructs::DetectionStruct m_detectionData = EnemyDataStructs::DetectionStruct();
	EnemyDataStructs::SummonStruct m_summonData = EnemyDataStructs::SummonStruct();
	EnemyDataStructs::MeleeStruct m_meleeData = EnemyDataStructs::MeleeStruct();
	EnemyDataStructs::RangedStruct m_rangedData = EnemyDataStructs::RangedStruct();
	void move();
	void charge(float _dt);
	void detect();
	void chargeMovement();
	void visible(float _dt);
	void shield(float _dt);
	void loadShieldImage();
	void mineLaying(float _dt);
	void blockLaying(float _dt);
	void spawnBlock();
	void spawnMine();
	void die() override;
	void generatePatrolPath(std::vector<int> _corners);
	void attack(float _dt);
	void summon(float _dt);
	void melee(float _dt);
	void ranged(float _dt);
	Tile* getFreeTileInRaidus(int _radius);
	void randomEnemy(int _difficulty);
	
public:
	struct PropertyChances {
			bool Movement = false;
			bool Charge = false;
			bool Shield = false;
			bool ShieldTimed = false;
			bool HealthRegen = false;
			bool Invisible = false;
			bool Flicker = false;
			bool Seek = false;
			bool Patrol = false;
			bool Contact = false;
			bool MineLayer = false;
			bool DodgeChance = false;
			bool Barrier = false;
			bool Detection = false;
			bool Summon = false;
			bool Melee = false;
			bool Ranged = false;
			bool PursueBeyondRoom = false;
			bool Passive = false;
	};
	Enemy();
	~Enemy();
	void Print();
	void Update(float _dt) override;
	void GenerateEnemy(int _difficulty, Scene* _scene, World* _world, RoomData& _roomSpawnedIn, PropertyChances* _chances = nullptr);
	bool TakeDamage(int _amount, DamageType _type) override;
	void Render(SDL_Renderer* _renderer) override;
	void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) override;
	void GenerateName(Scene* _scene);
};

