#include "Scene.h"
#include "Renderable.h"
#include "Tile.h"
#include <algorithm>
#include "Thing.h"
Scene::Scene(SDL_Renderer* _renderer)
{
	m_rendererRef = _renderer;
}
Scene::~Scene() {
	for (size_t i = 0; i < m_collisionChecks.size(); i++)
	{
		m_collisionChecks[i] = nullptr;
	}
}

void Scene::AddRenderable(const std::string _path, const std::string _name, float _x, float _y, float _renderDist)
{
	m_renderables.push_back(std::unique_ptr<Renderable>(new Renderable()));
	m_renderables[m_renderables.size() - 1]->Init(_path, _name, m_rendererRef);
	m_renderables[m_renderables.size() - 1]->SetPosition(_x, _y);
	addCollidable(m_renderables[m_renderables.size() - 1].get());
}


void Scene::AddRenderable(Renderable* _renderable, float _renderDist)
{
	m_renderables.push_back(std::unique_ptr<Renderable>(_renderable));
	addCollidable(_renderable);
	
}

void Scene::addCollidable(Renderable* _renderable)
{
	Thing* thing = dynamic_cast<Thing*>(_renderable);
	if (thing != nullptr)
	{
		m_collisionChecks.push_back(thing);
	}
}



void Scene::AddRenderable(const std::string _path, const std::string _name, Vector2 _position, float _renderDist)
{
	AddRenderable(_path, _name, (float)_position.X, (float)_position.Y, _renderDist);
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
	if (Renderable::renderOrderChanged())
	{
		std::sort(m_renderables.begin(), m_renderables.end(), [](std::unique_ptr<Renderable>& _lhs, std::unique_ptr<Renderable>& _rhs) { return _lhs.get()->GetRenderLayer() < _rhs.get()->GetRenderLayer(); });
		Renderable::renderOrderChanged() = false;
	}
	SDL_RenderClear(m_rendererRef);
	for (int i = 0; i < m_renderables.size(); i++)
	{
		m_renderables[i]->Render(m_rendererRef);
	}

	SDL_RenderPresent(m_rendererRef);
}

void Scene::Update(float _dt)
{
	for (int i = 0; i < m_renderables.size(); i++)
	{
		m_renderables[i]->Update(_dt);
	}

	for (size_t i = 0; i < m_collisionChecks.size(); i++)
	{
		if (m_collisionChecks[i]->ShouldCheckCollision())
		{
			for (size_t j = 0; j < m_collisionChecks.size(); j++)
			{
				if (m_collisionChecks[j]->IsSolid() && m_collisionChecks[j] != m_collisionChecks[i])
				{
					m_collisionChecks[i]->CheckCollision(m_collisionChecks[j]);
				}
			}
		}
	}
}

SDL_Renderer* Scene::GetRenderer() { return m_rendererRef; }