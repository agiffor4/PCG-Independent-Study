#include "Healthbar.h"

void Healthbar::Render(SDL_Renderer* _renderer)
{
	if (m_shouldRender)
	{
		if (_renderer != nullptr)
		{
			
			SDL_RenderCopyEx(_renderer, SDL_const_cast(SDL_Texture*, GetTexture()), NULL, &m_fill, 0, NULL, SDL_FLIP_NONE);
		}
		else
		{
			printf("MyRenderer for %s is null in Render function\n", m_name.c_str());
		}
	}
}

void Healthbar::SetHealthPercent(float _percent0To1)
{
	m_fill = GetDestination();
	m_fill.w *= _percent0To1;
}
