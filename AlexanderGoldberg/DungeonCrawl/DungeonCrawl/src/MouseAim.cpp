#include "MouseAim.h"
#include "SDL_mouse.h"
#include "SDL_Render.h"
MouseAim::MouseAim() {}
MouseAim::~MouseAim() {}
void MouseAim::SetOrigin(const Vector2& _position) {
	m_orignPosition = _position;
}
const Vector2 MouseAim::GetDirection() 
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	m_mousePos.X = x;
	m_mousePos.Y = y;
	return Vector2::GetDirection(m_mousePos, m_orignPosition);


}

void MouseAim::Render(SDL_Renderer* _renderer)
{/*
	int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_RenderDrawLine(_renderer, m_orignPosition.X, m_orignPosition.Y,x, y);*/
}
