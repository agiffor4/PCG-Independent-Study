#pragma once
#include "Renderable.h"
class Tile;
class Thing :
	public Renderable
{
private:
	int m_distanceCheckForCollisions = 32;
protected:
	Tile* m_location = nullptr;
	bool m_shouldDelete = true;
	bool m_checkCollision = false;
	bool m_solid = false;
public:
	Thing();
	~Thing();
	virtual void SetLocation(Tile* _newLocation);
	Tile* GetLocation();

	Vector2 GetPositionInGrid();
	int GetPositionInVector();
	bool ShouldDelete();
	bool IsSolid();
	bool ShouldCheckCollision();
	void CheckCollision(Thing* _other);
	virtual void OnCollision(Thing* _other);
};

