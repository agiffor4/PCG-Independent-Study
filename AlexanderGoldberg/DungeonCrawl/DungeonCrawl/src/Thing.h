#pragma once
#include "Renderable.h"
class Tile;
class Thing :
	public Renderable
{
protected:
	Tile* m_location = nullptr;
	bool m_shouldDelete = true;
public:
	Thing();
	~Thing();
	void SetLocation(Tile* _newLocation);
	Tile* GetLocation();
	bool ShouldDelete();

};

