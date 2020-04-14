#include "Enemy.h"
#include "World.h"
#include "Tile.h"
#include "Player.h"
#include <stack>
Enemy::Enemy()
{
	m_solid = true;
	
}

Enemy::~Enemy()
{
	if (m_shieldImage != nullptr)
	{
		delete(m_shieldImage);
	}
}

void Enemy::move()
{
	if (m_player == nullptr)
	{
		m_player = m_world->GetPlayer();
	}
	if (propertyInProfile(EnemyProperty::movemetMoves))
	{
		if (propertyInProfile(EnemyProperty::behaviorPatrol))
		{
			int targetIndex = m_behaviorData.PartolPointsAsIndexes[m_behaviorData.currentIndex];
			Tile* nearerTile = m_world->GetNeigborNearestTarget(GetLocation(), targetIndex, m_behaviorData.UseDiagonals);
			GetLocation()->MoveContentsTo(nearerTile);
			if (nearerTile->GetPositionInVector() == targetIndex)
			{
				m_behaviorData.IncrementIndex();
			}
		}
		if ((propertyInProfile(EnemyProperty::behaviorSeekout) || propertyInProfile(EnemyProperty::behaviorKeepDistance)))
		{
			

			if (m_detectionData.Detected)
			{
				

				if (m_movementData.RoomBound)
				{

					if (m_roomSpawnedIn.Contains(m_player->GetPositionInVector()))
					{
						if (propertyInProfile(EnemyProperty::behaviorKeepDistance))
						{
							int distance =Vector2::GetSquareDistance(m_player->GetPositionInGrid(), GetPositionInGrid());
							m_movementData.Moved = false;
							if (distance > m_behaviorData.MinMaxDist.Y)
							{
								std::vector<Tile*> neighbors = m_world->GetNeighbors(m_player->GetPositionInVector());
								int target = neighbors[rand() % neighbors.size()]->GetPositionInVector();
								Tile* nearerTile = m_world->GetNeigborNearestTarget(GetLocation(), target, m_behaviorData.UseDiagonals);
								moveToPositionIfFree(nearerTile);
									
							}
							else if (distance < m_behaviorData.MinMaxDist.X)
							{
								int target = m_player->GetPositionInVector();
								Tile* nearerTile = m_world->GetNeigborFurthestFromTarget(GetLocation(), target, m_behaviorData.UseDiagonals);
								moveToPositionIfFree(nearerTile);
									
							}
							else
							{
								int target = m_player->GetPositionInVector();
								bool moveCloser = getChance(50);
								Tile* t = nullptr;
								if (moveCloser)
									t = m_world->GetNeigborNearestTarget(GetLocation(), target, m_behaviorData.UseDiagonals);
								else
									t = m_world->GetNeigborFurthestFromTarget(GetLocation(), target, m_behaviorData.UseDiagonals);
								moveToPositionIfFree(t);
							}
						}
						else
						{
							std::vector<Tile*> neighbors = m_world->GetNeighbors(m_player->GetPositionInVector());
							int target = neighbors[rand() % neighbors.size()]->GetPositionInVector();
							Tile* nearerTile = m_world->GetNeigborNearestTarget(GetLocation(), target, m_behaviorData.UseDiagonals);
							moveToPositionIfFree(nearerTile);
						}
						
					}
				}
				else
				{
					if (m_world->FindPathFrom(GetLocation(), m_player->GetLocation()).size() > 0)
					{
						std::vector<Tile*> neighbors = m_world->GetNeighbors(m_player->GetLocation()->GetPositionInVector());
						int target = neighbors[rand() % neighbors.size()]->GetPositionInVector();
						Tile* nearerTile = m_world->GetNeigborNearestTarget(GetLocation(), target, m_behaviorData.UseDiagonals);
						moveToPositionIfFree(nearerTile);
					}
				}
			}
		}
	
	}
}

void Enemy::charge(float _dt)
{
	
	if (m_chargeData.ShouldChargeCheckIntervals.CountDown(_dt))
	{
		if (!m_chargeData.Charging && !m_chargeData.ChargeWindUpSec.GetShouldCountDown())
		{
			if (Vector2::GetDistanceLessThan(m_player->GetPositionInGrid(), GetPositionInGrid(), m_chargeData.MinDistance + 1))
			{
				return;
			}

			if (getChance(m_chargeData.ChanceToCarge))
			{
				//set charge direction and start windup
				m_chargeData.ChargeTargetIndex = m_player->GetPositionInVector();
				m_chargeData.DirectionOfCharge = (int)IsInLine1RelativeTo2(GetPositionInVector(), m_chargeData.ChargeTargetIndex);

				if (m_chargeData.DirectionOfCharge != -1)
				{
					m_chargeData.ChargeWindUpSec.SetShouldCountDown(true);
				}
			}

		}
	}

	if (m_chargeData.ChargeWindUpSec.CountDownAutoCheckBool(_dt))
	{
		m_chargeData.Charging = true;
	}

	if (m_chargeData.Charging)
	{
		if (m_chargeData.ChargeMovementIntervals.CountDown(_dt))
		{
			chargeMovement();
		}

	}
	
}

void Enemy::detect() 
{
	if (m_player == nullptr || m_location == nullptr)
	{
		m_detectionData.Detected = false;
		return;
	}
	if (!m_detectionData.Detected)
	{
		if (Vector2::GetDistanceLessThan(m_player->GetPositionInGrid(), GetPositionInGrid(), m_detectionData.DetectionRadius))
		{
			m_detectionData.Detected = true;
			printf("Detected is true\n");
		}
	}
	else
	{
		if (Vector2::GetDistanceGreaterThan(m_player->GetPositionInGrid(), GetPositionInGrid(), m_detectionData.DetectionRadiusLost))
		{
			m_detectionData.Detected = false;
			printf("Detected is false\n");
		}
	}
}
void Enemy::chargeMovement()
{
	Tile* t = m_world->GetAdjacentTile(GetPositionInVector(), (World::TileDirection)m_chargeData.DirectionOfCharge);
	if (!moveToPositionIfFree(t))
	{
		m_chargeData.Charging = false;
		if (m_chargeData.StunnedOnImpact)
		{
			//apply status effect
		}
	}
	bool atTarget = t->GetPositionInVector() == m_chargeData.ChargeTargetIndex;
	bool adjacentToTarget = m_world->TileIsAdajcent(m_chargeData.ChargeTargetIndex, t->GetPositionInVector());
	if (m_chargeData.StopAtTarget)
	{
		if (atTarget)
		{
			m_chargeData.Charging = false;
		}
	}

	if (!m_chargeData.Charging)
	{
		if (m_chargeData.DoesDamage)
		{
			if (m_world->TileIsAdajcent(GetPositionInVector(), m_player->GetPositionInVector()))
			{
				m_player->TakeDamage(m_chargeData.ChargeDamage, Enums::DamageType::Bludgeoning);
				
			}
		}
	}
	
}

void Enemy::visible(float _dt)
{
	switch (m_visibilityData.Status)
	{
	case EnemyDataStructs::VisibilityStruct::VisibleStatus::Visible:
		if (m_visibilityData.TimeVisible.CountDown(_dt))
		{
			m_visibilityData.LastStatus = EnemyDataStructs::VisibilityStruct::VisibleStatus::Visible;
			m_visibilityData.Status = EnemyDataStructs::VisibilityStruct::VisibleStatus::Flickering;
			if (!m_visibilityData.ConstTimes)
				m_visibilityData.TimeVisible.SetTimer(getRandominRange(m_visibilityData.MinMaxVisibleTime.X, m_visibilityData.MinMaxVisibleTime.Y));
		}
		break;
	case EnemyDataStructs::VisibilityStruct::VisibleStatus::Invisible:
		if (m_visibilityData.TimeInvisible.CountDown(_dt))
		{
			m_visibilityData.LastStatus = EnemyDataStructs::VisibilityStruct::VisibleStatus::Invisible;
			m_visibilityData.Status = EnemyDataStructs::VisibilityStruct::VisibleStatus::Flickering;
			if (!m_visibilityData.ConstTimes)
				m_visibilityData.TimeInvisible.SetTimer(getRandominRange(m_visibilityData.MinMaxInvisibleTime.X, m_visibilityData.MinMaxInvisibleTime.Y));
		}
		break;
	case EnemyDataStructs::VisibilityStruct::VisibleStatus::Flickering:
		if (m_visibilityData.FlickerRate.CountDown(_dt))
		{
			m_shouldRender = !m_shouldRender;
		}
		if (m_visibilityData.TimeFlickering.CountDown(_dt))
		{
			if (m_visibilityData.LastStatus == EnemyDataStructs::VisibilityStruct::VisibleStatus::Invisible)
			{
				m_shouldRender = true;
				m_visibilityData.Status = EnemyDataStructs::VisibilityStruct::VisibleStatus::Visible;
			}
			else
			{
				m_shouldRender = false;
				m_visibilityData.Status = EnemyDataStructs::VisibilityStruct::VisibleStatus::Invisible;
			}
			m_visibilityData.LastStatus = EnemyDataStructs::VisibilityStruct::VisibleStatus::Flickering;
			if (!m_visibilityData.ConstTimes)
				m_visibilityData.TimeFlickering.SetTimer(getRandominRange(m_visibilityData.MinMaxFlickerTime.X, m_visibilityData.MinMaxFlickerTime.Y));
		}
		break;
	default:
		break;
	}

}

void Enemy::shield(float _dt)
{
	if (propertyInProfile(EnemyProperty::defenseShieldBreakable))
	{
		if (m_shieldData.Regens)
		{
			if (m_shieldData.RechargeTimerSec.CountDown(_dt))
			{
				m_shieldData.Recharge();

			}
		}
	}

	if (propertyInProfile(EnemyProperty::defenseShieldTimed))
	{
		if (m_shieldData.TimeOn.CountDownAutoCheckBool(_dt))
		{
			m_shieldData.ShieldCurrent = 0;
			m_shieldData.TimeOff.SetShouldCountDown(true);
			if (m_shieldData.VariableTimes)
				m_shieldData.TimeOff.SetTimer(getRandominRange(m_shieldData.MinMaxTimeOff.X, m_shieldData.MinMaxTimeOff.Y));
		}
		if (m_shieldData.TimeOff.CountDownAutoCheckBool(_dt))
		{
			m_shieldData.ShieldCurrent = 1;
			m_shieldData.TimeOn.SetShouldCountDown(true);
			if (m_shieldData.VariableTimes)
				m_shieldData.TimeOn.SetTimer(getRandominRange(m_shieldData.MinMaxTimeOn.X, m_shieldData.MinMaxTimeOn.Y));
		}
	}
}

void Enemy::loadShieldImage()
{
	if (m_shieldImage == nullptr)
	{
		m_shieldImage = new Renderable();
		m_shieldImage->Init("img/pics/shield.png", "Shield", m_rendererRef);
		m_shieldImage->SetScale(GetScale());
	}
}

void Enemy::die()
{
	m_shouldRender = false;
	m_solid = false;
}



void Enemy::Update(float _dt)
{
	detect();
	if (!m_chargeData.Charging && !m_chargeData.ChargeWindUpSec.GetShouldCountDown() && m_movementData.MoveTimerSec.CountDown(_dt))
	{
		move();
	}
	if (propertyInProfile(EnemyProperty::behaviorCharge))
	{
		if (m_detectionData.Detected)
		{
			charge(_dt);
		}
	}

	if (propertyInProfile(EnemyProperty::visibilityFlicker))
	{
		visible(_dt);
	}

	shield(_dt);
}

void Enemy::GenerateEnemy(int _difficulty, World* _world, RoomData& _roomSpawnedIn)
{
	m_world = _world;
	m_roomSpawnedIn = _roomSpawnedIn;
	//addPropertyToProfile(EnemyProperty::movemetMoves);
	//addPropertyToProfile(EnemyProperty::behaviorSeekout);
	//addPropertyToProfile(EnemyProperty::visibilityFlicker);
	m_movementData.RoomBound = false;
	m_chargeData.MinDistance = 2;
	addPropertyToProfile(EnemyProperty::defenseShieldTimed);
	m_shieldData.TimeOff.SetShouldCountDown(true);
	loadShieldImage();
	
}

bool Enemy::TakeDamage(int _amount, DamageType _type)
{
	if (propertyInProfile(EnemyProperty::defenseShieldBreakable))
	{
		
		if (m_shieldData.ShieldCurrent <= 0)
		{
			m_shieldData.ShieldCurrent = 0;
			return Damagable::TakeDamage(_amount, _type);
		}
		m_shieldData.ShieldCurrent -= _amount;
		return false;
	}
	
	if (propertyInProfile(EnemyProperty::defenseShieldTimed))
	{
		if (m_shieldData.ShieldCurrent < 1)
			return Damagable::TakeDamage(_amount, _type);
		return false;
	}

}

void Enemy::Render(SDL_Renderer* _renderer)
{
	Renderable::Render(_renderer);
	if ((propertyInProfile(EnemyProperty::defenseShieldBreakable) || propertyInProfile(EnemyProperty::defenseShieldTimed)) && m_shieldData.ShieldCurrent > 0)
	{
		if (m_shieldImage != nullptr)
		{
			m_shieldImage->SetPosition(GetPosition());
			m_shieldImage->Render(_renderer);
		}
	}
}



int Enemy::getRandomInRange(int _min, int _max)
{
	if (_max < _min)
	{
		printf("_min out of bounds! returning -1.\n");
		return -1;
	}
	return (rand() % (_max + 1 - _min)) + _min;
}

float Enemy::getRandominRange(float _min, float _max)
{
	
	return ((getRandomInRange(_min * 1000, (_max-1) * 1000) - _min) / 1000) + _min;
}


bool Enemy::propertyInProfile(EnemyProperty _property)
{
	return (m_EnemyProfile & (Uint32)_property);
}

void Enemy::addPropertyToProfile(EnemyProperty _property)
{
	m_EnemyProfile |= (Uint32)_property;
}

void Enemy::removePropertyFromProfile(EnemyProperty _property)
{
	m_EnemyProfile &= ~(Uint32)_property;
}

Vector2 Enemy::rotateDirectionByDegrees(Vector2 _direction, float _degrees)
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

bool Enemy::getChance(int _percentChance)
{
	return (rand() % 100) < _percentChance;
}

bool Enemy::moveToPositionIfFree(Tile* _t)
{
	if (_t->IsPassable() && _t->GetPositionInVector() != m_player->GetPositionInVector())
	{
		m_movementData.Moved = true;
		GetLocation()->MoveContentsTo(_t);
		return true;
	}
	return false;
}

Enemy::Direction Enemy::IsInLine1RelativeTo2(int _point1, int _point2, int worldWdith)
{
	if (_point1 == _point2)
	{
		return Enemy::Direction::NONE;
	}
	if (worldWdith == - 1)
		worldWdith = m_world->GetMapDimentions().X;
	int t1X = m_world->GetTileAtIndex(_point1)->GetPositionInGrid().X;
	int t2X = m_world->GetTileAtIndex(_point2)->GetPositionInGrid().X;
	int t1Y = m_world->GetTileAtIndex(_point1)->GetPositionInGrid().Y;
	int t2Y = m_world->GetTileAtIndex(_point2)->GetPositionInGrid().Y;
	if (t1X == t2X)
	{
		
		if (_point1 < _point2)
		{
			return Enemy::Direction::DOWN;
		}
		else
		{
			return Enemy::Direction::UP; 
		}
	}
	if (t1Y == t2Y)
	{
		if (_point1 < _point2)
		{
			return Enemy::Direction::RIGHT;
		}
		else
		{
			return Enemy::Direction::LEFT;
		}
	}

	if (_point1 < _point2)
	{
		if (t1X < t2X)
		{
			if ((_point2 - _point1) % (worldWdith + 1) == 0)
			{
				return Enemy::Direction::DOWNRIGHT;
			}
		}
		else 
		{
			
			if ((_point2 - _point1) % (worldWdith - 1) == 0)
			{
				return Enemy::Direction::DOWNLEFT;
			}
		}

	}
	else
	{
		if (t1X < t2X)
		{
			if ((_point1 - _point2) % (worldWdith - 1) == 0)
			{
				return Enemy::Direction::UPRIGHT;
			}
		}
		else
		{
			
			if ((_point1 - _point2) % (worldWdith + 1) == 0)
			{
				return Enemy::Direction::UPLEFT;
			}
		}
	}

	return Enemy::Direction::NONE;
}
