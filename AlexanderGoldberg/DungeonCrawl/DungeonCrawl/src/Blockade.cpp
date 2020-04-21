#include "Blockade.h"
#include "Timer.h"
#include "Tile.h"
#include "Scene.h"
void Blockade::die()
{
	if (m_location != nullptr)
	{
		m_location->RemoveItem(this, true);
	}
}
Blockade::Blockade()
{
	m_lifetime = new Timer(1.0f);
}

Blockade::~Blockade()
{
	CleanUp();
}

void Blockade::CleanUp()
{
	if (m_lifetime != nullptr)
	{
		delete(m_lifetime);
	}
}

void Blockade::SetLifeTime(float _min, float _max)
{
	m_lifetime->SetTimer(((float)rand() / (float)RAND_MAX) * (_max - _min) + _min);
	m_lifetime->SetShouldCountDown(true);
}

void Blockade::Initialize()
{
	m_blocksPassage = true;
	m_shouldRender = true;
	m_solid = true;
}

void Blockade::Update(float _dt)
{
	if (m_lifetime->CountDownAutoCheckBool(_dt))
	{
		die();
	}
}

bool Blockade::Interaction(Thing* _thingInitatingInteraction)
{
	return false;
}
