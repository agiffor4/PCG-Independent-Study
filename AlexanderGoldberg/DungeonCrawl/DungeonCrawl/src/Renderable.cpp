#include "Renderable.h"

Renderable::Renderable()
{

}

Renderable::~Renderable() 
{
	CleanUp();
}
void Renderable::Init(const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor)
{
	m_name = _name;
	SDL_Surface* surface = SDL_LoadBMP(_path.c_str());
	if (surface != nullptr)
	{
		m_rendererRef = _renderer;
		SDL_SetColorKey(surface, SDL_TRUE, (_transparentColor == -999 ? SDL_MapRGB(surface->format, 0, 255, 0) : _transparentColor));
		m_shouldRender = true;
		m_texture = SDL_CreateTextureFromSurface(_renderer, surface);
		m_destination.w = surface->w;
		m_destination.h = surface->h;
		m_scaleDefault.X = surface->w;
		m_scaleDefault.Y = surface->h;
		
	}
	else
	{
		printf("Unable to load file at \"%s\"\n", _path.c_str());
	}
	SDL_FreeSurface(surface);
}

void Renderable::SetPosition(float _x, float _y) {
	m_position.X = _x;
	m_position.Y = _y;
	updateDestination();
}
void Renderable::SetPosition(Vector2& _pos) {
	m_position.X = _pos.X;
	m_position.Y = _pos.Y;
	updateDestination();
	
}

const Vector2& Renderable::GetPosition() { return m_position;}
SDL_Rect Renderable::GetDestination() { return m_destination; }

void Renderable::SetSize(Vector2& _scale) {

	m_scale.X = _scale.X;
	m_scale.Y = _scale.Y;
	updateScale();

}

Vector2 Renderable::GetCurrentSize() {

	return Vector2(m_destination.w, m_destination.h);

}
void Renderable::SetSize(float _x, float _y) {
	m_scale.X = _x;
	m_scale.Y = _y;
	updateScale();

}
void Renderable::updateDestination()
{
	m_destination.x = m_position.X;
	m_destination.y = m_position.Y;
}
void Renderable::updateScale()
{
	m_destination.w = m_scale.X;
	m_destination.h = m_scale.Y;
}
void Renderable::changeImage(std::string _imagePath, Uint32 _transparentColor)
{
	SDL_Surface* surface = SDL_LoadBMP(_imagePath.c_str());
	if (surface != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		SDL_SetColorKey(surface, SDL_TRUE, (_transparentColor == -999 ? SDL_MapRGB(surface->format, 0, 255, 0) : _transparentColor));
		m_texture = SDL_CreateTextureFromSurface(m_rendererRef, surface);
		m_destination.w = surface->w;
		m_destination.h = surface->h;
		m_scaleDefault.X = surface->w;
		m_scaleDefault.Y = surface->h;
		SetSize(m_scale);

	}
	else
	{
		printf("Unable to load file at \"%s\"\n", _imagePath.c_str());
	}
	SDL_FreeSurface(surface);
}

void Renderable::Render(SDL_Renderer* renderer)
{
	if (m_shouldRender)
	{
		if (renderer != nullptr)
		{
			SDL_RenderCopyEx(renderer, m_texture, NULL, &m_destination, ((int)m_currentAngle), NULL, SDL_FLIP_NONE);
		}
		else
		{
			printf("MyRenderer for %s is null in Render function\n", m_name.c_str());
		}
	}

}

void Renderable::CleanUp(){
	SDL_DestroyTexture(m_texture);
	printf("Clean up called for renderable %s\n", m_name.c_str());
}