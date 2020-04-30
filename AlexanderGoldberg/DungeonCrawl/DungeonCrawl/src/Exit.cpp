#include "Exit.h"
#include "World.h"
Exit::Exit(World* _world) 
{
	m_world = _world;
}
Exit::~Exit() {}

bool Exit::Interaction(Thing* _thingInitatingInteraction) {
	m_world->Generate();
	return false;
}