#pragma once
#include "Vector2.h"
#include "SDL.h"
#include <memory>

class Renderable
{
private:
	//basic funcationality
	Vector2 m_position = Vector2();	
	Vector2 m_scale = Vector2();
	Vector2 m_scaleDefault = Vector2();

	//SDL
	SDL_Texture* m_texture = nullptr;
	SDL_Rect m_destination = SDL_Rect();
	SDL_Renderer* m_rendererRef = nullptr;
	
	//rotation
	float m_currentAngle = 0;
	int m_lastCurrentAngle = 0;
protected:
	bool m_shouldRender = false;
	std::string m_name = "NO NAME SET";
	void updateDestination();
	void updateScale();
	//void changeImage(std::string _imagePath, Uint32 _transparentColor = -999);
public:
	void changeImage(std::string _imagePath, Uint32 _transparentColor = -999);
	Renderable();
	virtual ~Renderable();
	virtual void Init(const std::string _path, const std::string _name, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	virtual void Render(SDL_Renderer* renderer);
	void SetPosition(float x, float y);
	void SetPosition(Vector2& pos);
	const Vector2& GetPosition();
	SDL_Rect GetDestination();
	void SetSize(Vector2& _scale);
	void SetSize(float _x, float _y);
	Vector2 GetCurrentSize();
	const std::string& GetName();
	void CleanUp();
};

