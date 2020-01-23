#include "Tile.h"
Tile::Tile() {

}
Tile::~Tile() {
	Renderable::CleanUp();
}

void Tile::Init(const std::string _path, const std::string _name, int _currentIndex, int _xIndex, int _yIndex, Vector2 _startPos, SDL_Renderer* _renderer, Uint32 _transparentColor) {
	m_posInVector = _currentIndex;
	m_posInGrid.X = _xIndex;
	m_posInGrid.Y = _yIndex;
	Renderable::Init(_path, _name, _renderer, _transparentColor);
	SetPosition(_startPos);
}
void Tile::SetPassable(bool _val) {
	m_passable = _val;
}
bool Tile::IsPassable() { return m_passable; }