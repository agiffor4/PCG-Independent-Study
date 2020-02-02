#include "InputManager.h"


InputManager::InputManager() 
{
	//different vectors for keys up and down and mouse up and down
	m_subscribers[0] = std::vector<IInputHandler*>();
	m_subscribers[1] = std::vector<IInputHandler*>();
	m_subscribers[2] = std::vector<IInputHandler*>();
	m_subscribers[3] = std::vector<IInputHandler*>();
	
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
		case SDL_MOUSEBUTTONDOWN:
			onMouseDown(e.button);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseUp(e.button);
			break;
		default:
			break;
		}
	}
}



void InputManager::onKeyDown(SDL_Keycode _key) {
	for (int i = 0; i < m_subscribers[(int)KeyPressType::DOWN].size(); i++)
		m_subscribers[(int)KeyPressType::DOWN][i]->InvokeKeyDown(_key);
}
void InputManager::onKeyUp(SDL_Keycode _key) {
	for (int i = 0; i < m_subscribers[(int)KeyPressType::UP].size(); i++)
		m_subscribers[(int)KeyPressType::UP][i]->InvokeKeyUp(_key);
}


void InputManager::onMouseDown(SDL_MouseButtonEvent _mouse) {
	for (int i = 0; i < m_subscribers[(int)KeyPressType::MOUSEDOWN].size(); i++)
		m_subscribers[(int)KeyPressType::MOUSEDOWN][i]->InvokeMouseUpInternal(_mouse.button, _mouse.x, _mouse.y);
}

void InputManager::onMouseUp(SDL_MouseButtonEvent _mouse) {	
	for (int i = 0; i < m_subscribers[(int)KeyPressType::MOUSEUP].size(); i++)
		m_subscribers[(int)KeyPressType::MOUSEUP][i]->InvokeMouseDownInternal(_mouse.button, _mouse.x, _mouse.y);
}