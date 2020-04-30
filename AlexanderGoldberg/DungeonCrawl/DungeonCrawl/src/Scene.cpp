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
	
}

void Scene::AddRenderable(const std::string _path, const std::string _name, float _x, float _y, float _renderDist)
{
	m_renderables.push_back(std::unique_ptr<Renderable>(new Renderable()));
	m_renderables[m_renderables.size() - 1]->Init(_path, _name, m_rendererRef);
	m_renderables[m_renderables.size() - 1]->SetPosition(_x, _y);
	addCollidable(m_renderables[m_renderables.size() - 1].get());
	m_renderables[m_renderables.size() - 1]->SetScene(this);
}
void Scene::AddRenderable(Renderable*_renderable, float _renderDist)
{
	if (_renderable != nullptr)
	{
		_renderable->SetScene(this);
		m_renderables.push_back(std::unique_ptr<Renderable>(_renderable));
		addCollidable(_renderable);
	}
	
}

void Scene::AddCollidable(Renderable* _renderable)
{
	addCollidable(_renderable);
}

void Scene::RemoveRenderable(Renderable* _renderable)
{
	removeCollidable(_renderable);
	for (auto itt = m_renderables.begin(); itt != m_renderables.end(); itt++)
	{
		if ((*itt).get() == _renderable)
		{
			m_renderablesToDelete.push_back(std::move(*itt));
			
			m_renderables.erase(itt);
			break;
		}

	}
}

void Scene::AddUI(const std::string _path, const std::string _name, float _x, float _y, float _renderDist)
{
	m_UIElements.push_back(std::unique_ptr<Renderable>(new Renderable()));
	m_UIElements[m_UIElements.size() - 1]->Init(_path, _name, m_rendererRef);
	m_UIElements[m_UIElements.size() - 1]->SetPosition(_x, _y);
	m_UIElements[m_UIElements.size() - 1]->SetScene(this);
}

void Scene::AddUI(const std::string _path, const std::string _name, Vector2 _position, float _renderDist)
{
	AddUI(_path, _name, _position.X, _position.Y, _renderDist);
}

void Scene::AddUI(Renderable* _renderable, float _renderDist)
{
	if (_renderable != nullptr)
	{
		_renderable->SetScene(this);
		m_UIElements.push_back(std::unique_ptr<Renderable>(_renderable));
	}
}

void Scene::RemoveUI(Renderable* _renderable)
{
	for (auto itt = m_UIElements.begin(); itt != m_UIElements.end(); itt++)
	{
		if ((*itt).get() == _renderable)
		{
			
			m_UIElementsToDelete.push_back(std::move(*itt));
			m_UIElements.erase(itt);
			break;
		}

	}
}

void Scene::ClearCollidables()
{
	for (size_t i = 0; i < m_collisionChecks.size(); i++)
	{
		m_collisionChecks[i] = nullptr;
	}
	m_collisionChecks.clear();
}

void Scene::addCollidable(Renderable* _renderable)
{
	Thing* thing = dynamic_cast<Thing*>(_renderable);
	if (thing != nullptr)
	{
		m_collisionChecks.push_back(thing);
	}
}

void Scene::removeCollidable(Renderable* _renderable)
{
	Thing* toRemove = dynamic_cast<Thing*>(_renderable);
	for (auto itt = m_collisionChecks.begin(); itt != m_collisionChecks.end(); itt++)
	{
		if ((*itt) == toRemove)
		{
			m_collisionChecks.erase(itt);
			break;
		}

	}
}



void Scene::AddRenderable(const std::string _path, const std::string _name, Vector2 _position, float _renderDist)
{
	AddRenderable(_path, _name, (float)_position.X, (float)_position.Y, _renderDist);
}
void Scene::StartPlayingLastSound() {
}
void Scene::StartPlayingSoundAtIndex(int _index) {
	
}
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
	for (int i = 0; i < m_UIElements.size(); i++)
	{
		m_UIElements[i]->Render(m_rendererRef);
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
	m_renderablesToDelete.clear();
	m_UIElementsToDelete.clear();
}

SDL_Renderer* Scene::GetRenderer() { return m_rendererRef; }

