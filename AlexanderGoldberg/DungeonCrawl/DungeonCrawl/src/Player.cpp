#include "Player.h"
#include "Tile.h"
#include "World.h"
#include "InputManager.h"
#include "Scene.h"
#include "Interactable.h"
#include "Camera.h"
#include "Holdable.h"
Player::Player()
{
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::UP);
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::DOWN);
	m_shouldDelete = false;
	m_moveRate.SetTimer(0.2f);
	m_holdables[0] = nullptr;
	m_holdables[1] = nullptr;
}



Player::~Player()
{
	
	
}

void Player::Update(float _dt)
{

	

	m_moveRate.CountDownAutoCheckBool(_dt);
	if (!m_moveRate.GetShouldCountDown())
	{
		setDirectionFromFlagValues(m_directionKeyDownFlag);
		move(m_direction);
		m_moveRate.SetShouldCountDown(true);
	}
}

void Player::InvokeKeyDown(SDL_Keycode _key)
{
	
		switch (_key)
		{
		case SDLK_w:
			m_directionKeyDownFlag |= (Uint8)MovementDirection::UP;
			break;
		case SDLK_a:
			m_directionKeyDownFlag |= (Uint8)MovementDirection::LEFT;
			break;
		case SDLK_s:
			m_directionKeyDownFlag |= (Uint8)MovementDirection::DOWN;
			break;
		case SDLK_d:
			m_directionKeyDownFlag |= (Uint8)MovementDirection::RIGHT;
			break;
		default:
			break;
		}
	
}

void Player::InvokeKeyUp(SDL_Keycode _key)
{
	switch (_key)
	{
	case SDLK_w:
		m_directionKeyDownFlag ^= (Uint8)MovementDirection::UP;
		break;				   
	case SDLK_a:			   
		m_directionKeyDownFlag ^= (Uint8)MovementDirection::LEFT;
		break;				   
	case SDLK_s:			   
		m_directionKeyDownFlag ^= (Uint8)MovementDirection::DOWN;
		break;				   
	case SDLK_d:			   
		m_directionKeyDownFlag ^= (Uint8)MovementDirection::RIGHT;
		break;
	case SDLK_e:
		InteractWithThingInSpace();
		break;
	case SDLK_q:
		GetLocation()->PrintTileData();
		break;
	default:
		break;
	}
}

void Player::Initalize(World& _world, const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor)
{
	Renderable::Init(_path, _name, _renderer, _transparentColor);
	m_world = &_world;	
	SetRenderLayer(15);
	
}

void Player::InteractWithThingInSpace()
{
	if (m_location != nullptr && (m_inventory.size() < m_inventoryMax || m_inventoryMax == -1))
	{
		Interactable* retrievedItem = m_location->InteractWithItem();
		if (retrievedItem != nullptr)
		{
			Holdable* h = dynamic_cast<Holdable*>(retrievedItem);
			if (h != nullptr)
			{
				m_holdables[0] = h;
			}
			else
			{
				m_inventory.push_back(retrievedItem);
			}
		}
	}
}

void Player::attack(Vector2 _direction)
{
}


void Player::move(Vector2 _direction)
{
	

	if (!(_direction.X == 0 && _direction.Y == 0))
	{
		Tile* toMoveTo = m_world->GetAdjacentTile(GetLocation()->GetPositionInVector(), convertVectorToDirection(_direction));
		if (toMoveTo != nullptr && toMoveTo->IsPassable())
		{

			SetLineOfSight(false);
			GetLocation()->MoveContentsTo(toMoveTo);
			SetLocation(toMoveTo);
			for (size_t i = 0; i < 2; i++)
			{
				if (m_holdables[i] != nullptr)
				{
					m_holdables[i]->SetLocation(toMoveTo);
				}

			}
			SetLineOfSight(true);
			#if UseCamera == 1
				Vector2 adjustedDirection = Camera::ClampMovement(_direction);
				adjustedDirection.Y *= -1;
				Camera::SetOffset(Camera::Offset() + (adjustedDirection * m_location->GetCurrentSize().X));
			#endif
		}
	}
}

World::TileDirection Player::convertVectorToDirection(Vector2& const _toConvert)
{
	World::TileDirection direction = World::TileDirection::LEFT;
	if (_toConvert.X > 0 && _toConvert.Y > 0)
		direction = World::TileDirection::UPRIGHT;
	else if (_toConvert.X < 0 && _toConvert.Y > 0)
		direction = World::TileDirection::UPLEFT;
	else if (_toConvert.X > 0 && _toConvert.Y < 0)
		direction = World::TileDirection::DOWNRIGHT;
	else if (_toConvert.X < 0 && _toConvert.Y < 0)
		direction = World::TileDirection::DOWNLEFT;
	else if (_toConvert.X < 0)
		direction = World::TileDirection::LEFT;
	else if (_toConvert.X > 0)
		direction = World::TileDirection::RIGHT;
	else if (_toConvert.Y < 0)
		direction = World::TileDirection::DOWN;
	else if (_toConvert.Y > 0)
		direction = World::TileDirection::UP;

	return direction;
}

void Player::setDirectionFromFlagValues(Uint8 _flag)
{
	m_direction.X = 0;
	m_direction.Y = 0;
	if (_flag & (Uint8)MovementDirection::RIGHT)
		m_direction.X = 1;
	if (_flag & (Uint8)MovementDirection::LEFT)
		m_direction.X = -1;
	if (_flag & (Uint8)MovementDirection::LEFT && _flag & (Uint8)MovementDirection::RIGHT)
		m_direction.X = 0;
	
	if (_flag & (Uint8)MovementDirection::UP)
		m_direction.Y = 1;
	if (_flag & (Uint8)MovementDirection::DOWN)
		m_direction.Y = -1;
	if (_flag & (Uint8)MovementDirection::DOWN && _flag & (Uint8)MovementDirection::UP)
		m_direction.Y = 0;
		
}

void Player::SetLineOfSight(bool _inLineOfSight)
{
	auto tilesInRange = getTilesInLineOfSight(GetLocation());
	for (auto i = tilesInRange.begin(); i != tilesInRange.end(); i++)
	{
		(*i)->SetFogOfWar(!_inLineOfSight);
	}
}


std::set<Tile*> Player::getTilesInLineOfSight(Tile* _epicenter)
{
	std::set<Tile*> toIlluminate;
	int center = _epicenter->GetPositionInVector();

	std::vector<Tile*> neigbors = m_world->GetNeighbors(center);
	for (size_t i = 0; i < neigbors.size(); i++)
	{
		toIlluminate.emplace(neigbors[i]);
	}
	std::vector<Tile*> tempNeigbors;
	for (size_t k = 0; k < m_lineOfSightRadius; k++)
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