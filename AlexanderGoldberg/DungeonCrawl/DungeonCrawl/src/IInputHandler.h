#pragma once
#include "SDL_keycode.h"
class IInputHandler
{

public:
	IInputHandler();
	~IInputHandler();
	virtual void InvokeKeyDown(SDL_Keycode _key);
	virtual void InvokeKeyUp(SDL_Keycode _key);
};

