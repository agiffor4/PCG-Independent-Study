#include "Shadow.h"
#include "Tile.h"
#include "Camera.h"
#include "InputManager.h"
Shadow::Shadow()
{
	m_allowInteraction = false;
	InputManager::GetInputManager()->SubscribeToInput(this, InputManager::KeyPressType::MOUSEUP);
}

Shadow::~Shadow()
{
}

bool Shadow::Interaction()
{
	return false;
}
bool Shadow::inBounds(int _x, int _y) {
	if (_x > GetDestination().x - Camera::Offset().X && _x < GetDestination().x + GetDestination().w - Camera::Offset().X &&
		_y > GetDestination().y - Camera::Offset().Y && _y < GetDestination().y + GetDestination().h - Camera::Offset().Y)
	{
		return true;
	}
	return false;
}

void Shadow::InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y)
{
	switch (_mouse)
	{
	case IInputHandler::MouseButton::LEFT:
		break;
	case IInputHandler::MouseButton::RIGHT:
		if (inBounds(_x, _y))
		{
			
			printf("\nShadow is in %s\n", GetLocation() == nullptr ? "shadow location is null " : GetLocation()->GetName().c_str());
		}
		break;
	case IInputHandler::MouseButton::MIDDLE:
		break;
	default:
		break;
	}
}
