#pragma once
#include "Vector2.h"
struct SDL_Renderer;
class MouseAim 
{
private:
	Vector2 m_orignPosition;
	Vector2 m_mousePos;
public:
	MouseAim();
	~MouseAim();
	void SetOrigin(const Vector2& _position);	
	const Vector2 GetDirection();
	void Render(SDL_Renderer* _renderer);
};