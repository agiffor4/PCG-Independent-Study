#include "Exit.h"
#include "World.h"
Exit::Exit(World* _world) 
{
	m_world = _world;
}
Exit::~Exit() {}

bool Exit::Interaction() {
	m_world->GenerateLevel();
	return false;
}