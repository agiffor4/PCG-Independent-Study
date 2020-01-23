#pragma once
#include "Renderable.h"
class Tile :
	public Renderable
{
private:
	Vector2 m_posInGrid = Vector2();
	int m_posInVector = -1;
	bool m_passable = true;
public:
	Tile();
	~Tile();
	void Init(const std::string _path, const std::string _name, int _currentIndex, int _xIndex, int _yIndex, Vector2 _startPos, SDL_Renderer* _renderer, Uint32 _transparentColor = -999);
	bool IsPassable();
	void SetPassable(bool _val);
};

