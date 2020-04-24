#include "Renderable.h"
#include "Camera.h"
#include "SDL_image.h"

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
	SDL_Surface* surface = IMG_Load(_path.c_str());
	if (surface != nullptr)
	{
		m_rendererRef = _renderer;
		SDL_SetColorKey(surface, SDL_TRUE, (_transparentColor == -999 ? SDL_MapRGB(surface->format, 0, 255, 0) : _transparentColor));
		m_shouldRender = true;
		m_texture = SDL_CreateTextureFromSurface(_renderer, surface);
		m_destination.w = surface->w;
		m_destination.h = surface->h;
		m_defaultSize.X = surface->w;
		m_defaultSize.Y = surface->h;
		
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
	SetPosition(_pos.X, _pos.Y);
}

void Renderable::SetPosition(const Vector2& _pos) {
	SetPosition(_pos.X, _pos.Y);

}

const Vector2& Renderable::GetPosition() { return m_position;}
const Vector2 Renderable::GetCameraAdjustPosition(bool _centered)
{
	SDL_Rect cameraAdjustedDestination = m_destination;
	cameraAdjustedDestination.x -= Camera::Offset().X;
	cameraAdjustedDestination.y -= Camera::Offset().Y;
	cameraAdjustedDestination.x += m_renderOffset.X;
	cameraAdjustedDestination.y += m_renderOffset.Y;
	
	return Vector2(cameraAdjustedDestination.x + (_centered ? m_destination.w * 0.5f : 0), cameraAdjustedDestination.y + (_centered ? m_destination.h * 0.5f : 0));
}
SDL_Rect Renderable::GetDestination() { return m_destination; }

void Renderable::SetScale(float _newScale)
{
	m_scale = _newScale;
	m_currentSize.X = m_defaultSize.X * m_scale;
	m_currentSize.Y = m_defaultSize.Y * m_scale;
	updateScale();
}

float Renderable::GetScale()
{
	return m_scale;
}

void Renderable::SetSize(Vector2& _scale) {

	SetSize(_scale.X, _scale.Y);


}

Vector2 Renderable::GetCurrentSize() {

	return Vector2(m_destination.w, m_destination.h);

}
const Vector2& Renderable::GetDefaultSize()
{
	return m_defaultSize;
}
const std::string& Renderable::GetName()
{
	return m_name;
}
const SDL_Texture* Renderable::GetTexture()
{
	return m_texture;
}
void Renderable::SetSize(float _x, float _y) {
	m_currentSize.X = _x;
	m_currentSize.Y = _y;
	updateScale();

}
void Renderable::updateDestination()
{
	m_destination.x = m_position.X;
	m_destination.y = m_position.Y;
}
void Renderable::updateScale()
{
	m_destination.w = m_currentSize.X;
	m_destination.h = m_currentSize.Y;
}
void Renderable::storeTextureColorMod()
{
	getTextureColorMod(m_colorMod);
}
void Renderable::revertTextureColorMod()
{
	setTextureColorMod(m_colorMod); 
}
void Renderable::getTextureColorMod(SDL_Color& _color)
{
	SDL_GetTextureColorMod(m_texture, &_color.r, &_color.g, &_color.b);
}
void Renderable::setTextureColorMod(SDL_Color _color)
{
	SDL_SetTextureColorMod(m_texture, _color.r, _color.g, _color.b);
}
const Vector2 Renderable::getCenterOfTexture()
{
	return Vector2(m_destination.x  + (m_destination.w *0.5f), m_destination.y + (m_destination.h * 0.5f));
}
void Renderable::setAngle(float _angle)
{
	m_currentAngle = _angle;
}
float Renderable::vectorToAngle(Vector2 _direction)
{
	float angle = (acosf(Vector2::Dot(Vector2(0, -1), _direction)) * rad2Deg);
	if (_direction.X < 0)
	{
		angle = 360 - angle;
	}
	return angle;
}
bool& Renderable::renderOrderChanged(){
	static bool s_renderOrderChanged;
	return s_renderOrderChanged;
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
		m_defaultSize.X = surface->w;
		m_defaultSize.Y = surface->h;
		SetSize(m_currentSize);

	}
	else
	{
		printf("Unable to load file at \"%s\"\n", _imagePath.c_str());
	}
	SDL_FreeSurface(surface);
}

void Renderable::SetRenderLayer(int _renderLayer)
{
	m_renderLayer = _renderLayer;
	renderOrderChanged() = true;
}

int Renderable::GetRenderLayer()
{
	return m_renderLayer;
}

void Renderable::Render(SDL_Renderer* renderer)
{
	if (m_shouldRender)
	{
		if (renderer != nullptr)
		{
			SDL_Rect cameraAdjustedDestination = m_destination;
			cameraAdjustedDestination.x -= Camera::Offset().X;
			cameraAdjustedDestination.y -= Camera::Offset().Y;
			cameraAdjustedDestination.x += m_renderOffset.X;
			cameraAdjustedDestination.y += m_renderOffset.Y;
			SDL_RenderCopyEx(renderer, m_texture, NULL, &cameraAdjustedDestination, ((int)m_currentAngle), NULL, SDL_FLIP_NONE);
		}
		else
		{
			printf("MyRenderer for %s is null in Render function\n", m_name.c_str());
		}
	}

}

void Renderable::Update(float _dt)
{
}

void Renderable::CleanUp(){
	SDL_DestroyTexture(m_texture);
	//printf("Clean up called for renderable %s\n", m_name.c_str());
}


void Renderable::SetRenderableOffset(Vector2 _renderOffset)
{
	m_renderOffset = _renderOffset;
}
const Vector2& Renderable::GetRenderableOffSet()
{
	return m_renderOffset;
}