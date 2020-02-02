#pragma once
#include "SDL_keycode.h"


//#include "SDL_events.h"
class IInputHandler
{

public:
	enum class MouseButton {
		LEFT,
		RIGHT,
		MIDDLE
	};
	IInputHandler();
	~IInputHandler();
	virtual void InvokeKeyDown(SDL_Keycode _key);
	virtual void InvokeKeyUp(SDL_Keycode _key);
	virtual void InvokeMouseDown(MouseButton _mouse, Sint32 _x, Sint32 _y);
	virtual void InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y);
	void InvokeMouseDownInternal(Uint8 _mouse, Sint32 _x, Sint32 _y);
	void InvokeMouseUpInternal(Uint8 _mouse, Sint32 _x, Sint32 _y);
	

};

