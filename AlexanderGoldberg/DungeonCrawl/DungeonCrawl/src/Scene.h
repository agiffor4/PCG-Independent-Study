#pragma once
#include "SDL.h"
#include <memory>
#include <Vector>
#include <string>
#include "Vector2.h"
class Renderable;
class Tile;

class Scene {
protected:
	std::vector<std::unique_ptr<Renderable>> m_renderables;
	SDL_Renderer* m_rendererRef = nullptr;
public:
	
	Scene(SDL_Renderer* _renderer);
	~Scene();
	void AddRenderable(const std::string _path, const std::string _name, float _x = 0, float _y = 0);
	void AddRenderable(const std::string _path, const std::string _name, Vector2 _position);
	void AddRenderable(Renderable* _renderable);


	//int AddSound(AudioFileA& _sound, bool _playOnCreate);
	void StartPlayingLastSound();
	void StartPlayingSoundAtIndex(int _index);
	//AudioFileA* GetLastSoundObject();
	//AudioFileA* GetSoundAtObject(int _index);
	void Render();
	void PlayAudio();
	SDL_Renderer* GetRenderer();
	
};