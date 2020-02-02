#include "IInputHandler.h"
#include "SDL_mouse.h"
IInputHandler::IInputHandler()
{

}


IInputHandler::~IInputHandler()
{
	
}

void IInputHandler::InvokeKeyDown(SDL_Keycode _key) {}
void IInputHandler::InvokeKeyUp(SDL_Keycode _key) {}
void IInputHandler::InvokeMouseDown(MouseButton _mouse, Sint32 _x, Sint32 _y) {}
void IInputHandler::InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) {}
void IInputHandler::InvokeMouseDownInternal(Uint8 _mouse, Sint32 _x, Sint32 _y) {

	
	switch (_mouse)
	{
	case SDL_BUTTON_LEFT:
		InvokeMouseUp(MouseButton::LEFT, _x, _y);
		break;
	case SDL_BUTTON_MIDDLE:
		InvokeMouseUp(MouseButton::MIDDLE, _x, _y);
		break;
	case SDL_BUTTON_RIGHT:
		InvokeMouseUp(MouseButton::RIGHT, _x, _y);
		break;
	default:
		break;
	}
}
void IInputHandler::InvokeMouseUpInternal(Uint8 _mouse, Sint32 _x, Sint32 _y) {
	switch (_mouse)
	{
	case SDL_BUTTON_LEFT:
		InvokeMouseDown(MouseButton::LEFT, _x, _y);
		break;
	case SDL_BUTTON_MIDDLE:
		InvokeMouseDown(MouseButton::MIDDLE, _x, _y);
		break;
	case SDL_BUTTON_RIGHT:
		InvokeMouseDown(MouseButton::RIGHT, _x, _y);
		break;
	default:
		break;
	}
}

