#pragma once
#include "Renderable.h"
class Healthbar :
	public Renderable
{
protected:
	SDL_Rect m_fill;
public:
	void Render(SDL_Renderer* _renderer) override;
	void SetHealthPercent(float _percent0To1);
};

