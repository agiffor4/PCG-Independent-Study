#include "Tile.h"
#include "World.h"
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
	if (m_passable)
	{
		changeImage("img/blank_tile.bmp");
	}
	else
	{
		changeImage("img/block_tile.bmp");
	}
}
bool Tile::IsPassable() { return m_passable; }

bool Tile::inBounds(int _x, int _y) {
	if (_x > GetPosition().X && _x < GetPosition().X + GetDestination().w &&
		_y > GetPosition().Y && _y < GetPosition().Y + GetDestination().h)
	{
		return true;
	}
	return false;
}
int Tile::GetPositionInVector() { return m_posInVector; }
const Vector2& Tile::GetPositionInGrid() { return m_posInGrid; }



void Tile::InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) 
{
	
	switch (_mouse)
	{
	case IInputHandler::MouseButton::LEFT:
		if (inBounds(_x, _y))
		{
			printf("clicked on tile %s, setting passable to %s\n", m_name.c_str(), (!IsPassable() ? "true." : "false."));
			SetPassable(!IsPassable());
		}
		break;
	case IInputHandler::MouseButton::RIGHT:
		break;
	case IInputHandler::MouseButton::MIDDLE:
		break;
	default:
		break;
	}
}