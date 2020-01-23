#pragma once
#include "IInputHandler.h"
class Quit :
	public IInputHandler
{
private:
	bool* m_play = nullptr;
	void quit();
public:
	Quit(bool* _falseify);
	void InvokeKeyDown(SDL_Keycode _key) override;
	void InvokeKeyUp(SDL_Keycode _key) override;
	
};