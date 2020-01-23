#pragma once
#include <SDL.h>
#include <vector>
#include "IInputHandler.h"
class InputManager
{

	
private:
	
	SDL_Event e = SDL_Event();
	void onKeyDown(SDL_Keycode _key);
	void onKeyUp(SDL_Keycode _key);

	std::vector<IInputHandler*> m_subscribers[2];
	
public:
	enum class KeyPressType {
		UP,
		DOWN,

	};
	InputManager();
	~InputManager();
	void CheckInput();
	
	template <typename T> void SubscribeToInput(T* _func, KeyPressType _upOrDown) {
		IInputHandler* ih = (IInputHandler*)(_func);
		if (ih != nullptr)
			m_subscribers[(int)_upOrDown].push_back(ih);
		else
			printf("Unable to cast pointer to IInputHandler.  Not subscribing to InputManager.");
	}

	static InputManager* GetInputManager() {
		static InputManager singleton;
		return &singleton;
	}



};

