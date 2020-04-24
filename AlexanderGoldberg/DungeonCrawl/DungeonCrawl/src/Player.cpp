#include "Player.h"
#include "Tile.h"
#include "World.h"
#include "InputManager.h"
#include "Interactable.h"
#include "Camera.h"
#include "Holdable.h"
#include "Weapon.h"
#include "PlayerHUD.h"
Player::Player()
{
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::UP);
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::DOWN);
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::MOUSEUP);
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::MOUSEDOWN);
	m_shouldDelete = false;
	m_moveRate.SetTimer(0.2f);
	m_holdables[0] = nullptr;
	m_holdables[1] = nullptr;
	SetHealthMax(100);
	
}



Player::~Player()
{
	
	if (m_hud != nullptr)
		delete(m_hud);
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
	if (m_equipedWeapon != nullptr)
	{
		m_equipedWeapon->Update(_dt);
		if (m_equipedWeapon->ShouldSetLOS())
		{
			SetLineOfSight(true);
		}
	}
	
	if (m_fireButtonDown)
	{
		if (m_equipedWeapon != nullptr)
		{
			setAimDirection(m_mouseAim.GetDirection());
			m_equipedWeapon->Fire(GetAimDirection());
		}
	}
	m_hud->Update(_dt);
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

void Player::InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y)
{
	switch (_mouse)
	{
	case IInputHandler::MouseButton::LEFT:		
		m_fireButtonDown = false;
		break;
	case IInputHandler::MouseButton::RIGHT:
		break;
	case IInputHandler::MouseButton::MIDDLE:
		break;
	default:
		break;
	}
}

void Player::InvokeMouseDown(MouseButton _mouse, Sint32 _x, Sint32 _y)
{
	switch (_mouse)
	{
	case IInputHandler::MouseButton::LEFT:
		m_fireButtonDown = true;
		break;
	case IInputHandler::MouseButton::RIGHT:
		break;
	case IInputHandler::MouseButton::MIDDLE:
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
	SetName("Player");
	m_hud = new PlayerHUD(this, _renderer);
	
}

void Player::InteractWithThingInSpace()
{
	if (m_location != nullptr && (m_inventory.size() < m_inventoryMax || m_inventoryMax == -1))
	{
		Interactable* retrievedItem = m_location->InteractWithItem();
		if (retrievedItem != nullptr)
		{
			
			bool actionPerformed = false;
			if (!actionPerformed)
			{
				Holdable* h = dynamic_cast<Holdable*>(retrievedItem);
				if (h != nullptr)
				{
					if (m_holdables[0] == nullptr)
					{
						m_holdables[0] = h;
					}
					else if (m_holdables[1] == nullptr)
					{
						m_holdables[1] = h;
					}
					else
					{
						dropHoldable();
					}
					actionPerformed = true;
				}
			}
			if (!actionPerformed)
			{
				if (dropWeapon())
				{
					Weapon* w = dynamic_cast<Weapon*>(retrievedItem);
					w->SetHolder(this);
					w->PickUp();
					EquipWeapon(w);
				}
			}
			if (!actionPerformed)
			{

			}
			
			if(!actionPerformed)
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

void Player::dropHoldable()
{
	Tile* t = findEmptyAdjacentSpace();
	if (t != nullptr)
	{
		t->AddItem(m_holdables[0]);
		m_holdables[0] = nullptr;
	}
}

bool Player::dropWeapon()
{
	if (m_equipedWeapon != nullptr)
	{
		Tile* t = findEmptyAdjacentSpace();
		if (t != nullptr)
		{
			m_equipedWeapon->Drop();
			t->AddItem(m_equipedWeapon);
			m_equipedWeapon->SetHolder(nullptr);
			EquipWeapon(nullptr);
			return true;
		}
		return false;
	}
	return true;
}

Tile* Player::findEmptyAdjacentSpace()
{
	std::vector<Tile*> negihbors = m_world->GetNeighbors(GetLocation());
	Tile* t = GetLocation();
	bool checked[8] = { false, false, false, false, false, false, false, false };
	auto allHaveBeenChecked = [](bool* _array, int _index)
	{
		
		for (size_t i = 0; i < _index; i++)
		{
			if (!_array[i]) //if any of the array elements have not been checked (equal false) then return false
				return false;
		}
		return true;
	};
	int index = 0;
	bool noEmptySpace = false;
	do
	{
		
		
		index = rand() % negihbors.size();
		if (!checked[index])
		{
			checked[index] = true;
			t = negihbors[index];
		}
		else
		{
			if (allHaveBeenChecked(checked, 8))
			{
				return nullptr;
			}
		}
	} while (t->GetItems().size() > 0 || !t->IsPassable());

	return t;
}

void Player::SetLineOfSight(bool _inLineOfSight)
{
	auto tilesInRange = getTilesInLineOfSight(GetLocation());
	for (auto i = tilesInRange.begin(); i != tilesInRange.end(); i++)
	{
		int dist = Vector2::GetMagnitude(GetLocation()->GetPositionInGrid(), (*i)->GetPositionInGrid());
		(*i)->SetFogOfWar(!_inLineOfSight, (_inLineOfSight ?  dist : -1));
	}
}

bool Player::AddAmmo(int _amount)
{
	if (m_equipedWeapon != nullptr)
	{
		return m_equipedWeapon->AddAmmo(_amount);
	}
	return false;
}

void Player::Render(SDL_Renderer* _renderer)
{
	Renderable::Render(_renderer);
	Vector2 v = GetCameraAdjustPosition(true);
	m_mouseAim.SetOrigin(v);
	m_mouseAim.Render(_renderer);
	m_hud->Render(_renderer);
}

void Player::EquipWeapon(Weapon* _weapon)
{
	m_equipedWeapon = _weapon;
	if (m_equipedWeapon != nullptr)
	{
		printf("\n\n\n");
		m_equipedWeapon->PrintWeaponInfo();
		printf("\n\n\n");
	}
}

int Player::GetAmmo()
{
	return m_equipedWeapon == nullptr ? -1 : m_equipedWeapon->GetAmmo();
}

float Player::GetAmmoAsPercent()
{
	return m_equipedWeapon != nullptr ? m_equipedWeapon->GetAmmoAsPercent() : 0.0f;
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
void Player::die() {
}