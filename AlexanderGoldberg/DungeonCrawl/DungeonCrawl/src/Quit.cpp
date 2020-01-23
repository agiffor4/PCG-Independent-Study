#include "Quit.h"
#include "SDL.h"
void Quit::quit() {
	(*m_play) = false;
}

Quit::Quit(bool* _falseify) {
	m_play = _falseify;
}

void Quit::InvokeKeyUp(SDL_Keycode _key) {}
void Quit::InvokeKeyDown(SDL_Keycode _key) {
	switch (_key)
	{
	case SDLK_ESCAPE:
		quit();
		break;
	default:
		break;
	}
}