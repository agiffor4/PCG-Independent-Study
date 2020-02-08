#include "Player.h"
#include "Tile.h"
#include "World.h"
#include "InputManager.h"
#include "Scene.h"
Player::Player()
{
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::UP);
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::DOWN);
}



Player::~Player()
{
}

void Player::InvokeKeyDown(SDL_Keycode _key)
{
}

void Player::InvokeKeyUp(SDL_Keycode _key)
{
	switch (_key)
	{
	case SDLK_w:
		move(Vector2(0, 1));
		break;
	case SDLK_a:
		move(Vector2(-1, 0));
		break;
	case SDLK_s:
		move(Vector2(0, -1));
		break;
	case SDLK_d:
		move(Vector2(1, 0));
		break;
	default:
		break;
	}
}

void Player::Initalize(Scene* _Scene, World& _world, const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor)
{
	Renderable::Init(_path, _name, _renderer, _transparentColor);
	_Scene->AddRenderable(this);
	m_world = &_world;
	SetSize(m_world->GetTileSize().X, m_world->GetTileSize().Y);
}

void Player::attack(Vector2 _direction)
{
}

void Player::move(Vector2 _direction)
{
	

	Tile* toMoveTo = m_world->GetAdjacentTile(GetLocation()->GetPositionInVector(), convertVectorToDirection(_direction));
	if (toMoveTo != nullptr && toMoveTo->IsPassable())
	{
		GetLocation()->MoveContentsTo(toMoveTo);
		SetLocation(toMoveTo);
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
