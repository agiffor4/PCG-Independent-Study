#pragma once
#include "SDL.h"
#include <memory>
#include <Vector>
#include <string>
#include "Vector2.h"
class Renderable;
class Tile;
class Thing;
class Scene {
protected:
	
	std::vector<std::unique_ptr<Renderable>> m_renderables;
	std::vector<Thing*> m_collisionChecks;
	SDL_Renderer* m_rendererRef = nullptr;
	void addCollidable(Renderable* _renderable);
	void removeCollidable(Renderable* _renderable);
public:
	
	Scene(SDL_Renderer* _renderer);
	~Scene();
	void AddRenderable(const std::string _path, const std::string _name, float _x = 0, float _y = 0, float _renderDist = 0);
	void AddRenderable(const std::string _path, const std::string _name, Vector2 _position, float _renderDist = 0);
	void AddRenderable(Renderable* _renderable, float _renderDist = 0);
	void AddCollidable(Renderable* _renderable);
	void RemoveRenderable(Renderable* _renderable);
	void ClearCollidables();
	//int AddSound(AudioFileA& _sound, bool _playOnCreate);
	void StartPlayingLastSound();
	void StartPlayingSoundAtIndex(int _index);
	//AudioFileA* GetLastSoundObject();
	//AudioFileA* GetSoundAtObject(int _index);
	void Render();
	void Update(float _dt);
	void PlayAudio();
	SDL_Renderer* GetRenderer();

};