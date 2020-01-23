#include "Scene.h"
#include "Renderable.h"
#include "Tile.h"
Scene::Scene(SDL_Renderer* _renderer) 
{
	m_rendererRef = _renderer;
}
Scene::~Scene() {}

void Scene::AddRenderable(const std::string _path, const std::string _name, float _x, float _y)
{
	m_renderables.push_back(std::unique_ptr<Renderable>(new Renderable()));
	m_renderables[m_renderables.size() - 1]->Init(_path, _name, m_rendererRef);
	m_renderables[m_renderables.size() - 1]->SetPosition(_x, _y);
}


void Scene::AddRenderable(Renderable* _renderable)
{
	m_renderables.push_back(std::unique_ptr<Renderable>(_renderable));
}

void Scene::AddRenderable(const std::string _path, const std::string _name, Vector2 _position)
{
	m_renderables.push_back(std::unique_ptr<Renderable>(new Renderable()));
	m_renderables[m_renderables.size() - 1]->Init(_path, _name, m_rendererRef);
	m_renderables[m_renderables.size() - 1]->SetPosition(_position.X, _position.Y);
}
/*
int Scene::AddSound(AudioFileA& _sound, bool _playOnCreate)
{
	
	return 0;
}
*/
void Scene::StartPlayingLastSound() {
}
void Scene::StartPlayingSoundAtIndex(int _index) {
	
}
/*
AudioFileA* Scene::GetLastSoundObject() {
	if (sounds.size() > 0)
		return sounds[sounds.size() - 1];
	else
		return nullptr;
}
AudioFileA* Scene::GetSoundAtObject(int _index) {
	if (_index < sounds.size())
	{
		return sounds[_index];
	}
	else
	{
		printf("Index out of range cannot play audio at index %d.  Returning nullptr.", _index);
		return nullptr;
	}
		
}
*/
void Scene::PlayAudio() {

	
	
}


void Scene::Render()
{
	SDL_RenderClear(m_rendererRef);
	for (int i = 0; i < m_renderables.size(); i++)
	{
		m_renderables[i]->Render(m_rendererRef);
	}

	SDL_RenderPresent(m_rendererRef);
}

SDL_Renderer* Scene::GetRenderer() { return m_rendererRef; }