#pragma once
#include "Renderable.h"
class Tile;
class Thing :
	public Renderable
{
protected:
	Tile* m_location = nullptr;
public:
	Thing();
	~Thing();
	void SetLocation(Tile* _newLocation);
	Tile* GetLocation();

};

