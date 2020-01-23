#include "InputManager.h"


InputManager::InputManager() 
{
	//for key up
	m_subscribers[0] = std::vector<IInputHandler*>();
	//for key down
	m_subscribers[1] = std::vector<IInputHandler*>();
	
}
InputManager::~InputManager() {}
void InputManager::CheckInput()
{
	while (SDL_PollEvent(&e))
	{
		
		switch (e.type)
		{
		case SDL_KEYDOWN:
			onKeyDown(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			onKeyUp(e.key.keysym.sym);
			break;
		default:
			break;
		}
	}
}



void InputManager::onKeyDown(SDL_Keycode _key) {
	for (int i = 0; i < m_subscribers[1].size(); i++)
		m_subscribers[1][i]->InvokeKeyDown(_key);
}
void InputManager::onKeyUp(SDL_Keycode _key) {
	for (int i = 0; i < m_subscribers[0].size(); i++)
		m_subscribers[0][i]->InvokeKeyUp(_key);
}