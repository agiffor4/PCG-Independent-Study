#pragma once
#include "Vector2.h"
#include "SDL.h"
#include <memory>

class Renderable
{
private:
	
	//basic funcationality
	Vector2 m_position = Vector2();	
	Vector2 m_currentSize = Vector2();
	Vector2 m_renderOffset = Vector2();
	float m_scale = 1;
	Vector2 m_defaultSize = Vector2();
	int m_renderLayer = 0;
	//SDL
	SDL_Texture* m_texture = nullptr;
	SDL_Rect m_destination = SDL_Rect();
	SDL_Color m_colorMod = { 255, 255, 255, 255 };
	
	//rotation
	float m_currentAngle = 0;
	int m_lastCurrentAngle = 0;
protected:
	bool m_shouldRender = false;
	std::string m_name = "NO NAME SET";
	void updateDestination();
	void updateScale();
	SDL_Renderer* m_rendererRef = nullptr;
	void storeTextureColorMod();
	void revertTextureColorMod();
	void getTextureColorMod(SDL_Color& _color);
	void setTextureColorMod(SDL_Color _color);
	//void changeImage(std::string _imagePath, Uint32 _transparentColor = -999);
public:
	static bool& renderOrderChanged();
	void changeImage(std::string _imagePath, Uint32 _transparentColor = -999);
	void SetRenderLayer(int _renderLayer);
	int GetRenderLayer();
	Renderable();
	virtual ~Renderable();
	virtual void Init(const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	virtual void Render(SDL_Renderer* renderer);
	virtual void Update(float _dt);
	void SetPosition(float x, float y);
	void SetPosition(Vector2& pos);
	void SetPosition(const Vector2& pos);
	const Vector2& GetPosition();
	SDL_Rect GetDestination();
	void SetScale(float _newScale);
	float GetScale();
	void SetSize(Vector2& _scale);
	void SetSize(float _x, float _y);
	Vector2 GetCurrentSize();
	const Vector2& GetDefaultSize();
	void SetRenderableOffset(Vector2 _renderOffset);
	const Vector2& GetRenderableOffSet();
	const std::string& GetName();
	virtual void CleanUp();

};

