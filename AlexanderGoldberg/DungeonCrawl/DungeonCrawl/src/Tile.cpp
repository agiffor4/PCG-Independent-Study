#include "Tile.h"
#include "Thing.h"
#include "World.h"
#include "Interactable.h"
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
void Tile::SetCorridor(bool _val)
{
	m_corridor = _val;
}
bool Tile::IsCorridor()
{
	return m_corridor;
}
void Tile::SetContents(Thing* _newContents)
{
	/*
	if (m_contents != nullptr)
	{
		if (m_contents->ShouldDelete())
			delete(m_contents);
		else
			m_contents->SetLocation(nullptr);
	}*/
	
	m_contents = _newContents;
	
}
Thing* Tile::GetContents()
{
	return m_contents;
}
void Tile::MoveContentsTo(Tile* _newLocation)
{
	_newLocation->SetContents(m_contents);
	SetContents(nullptr);
}
bool Tile::IsPassable(bool _ignoreInteractables) { return m_passable && (_ignoreInteractables ? true : blockingInteractable()); }

bool Tile::blockingInteractable() {
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->GetBlocksPassage())
			return false;
	}
	return true;
}

bool Tile::inBounds(int _x, int _y) {
	if (_x > GetPosition().X && _x < GetPosition().X + GetDestination().w &&
		_y > GetPosition().Y && _y < GetPosition().Y + GetDestination().h)
	{
		return true;
	}
	return false;
}
int Tile::GetPositionInVector() { return m_posInVector; }
Interactable* Tile::InteractWithItem()
{	
	if (!m_items.empty())
	{
		Interactable* toReturn = m_items[m_items.size() -1];
		if (toReturn->InteratctionWrapper()) // if returns true then adds to inventory
		{
			m_items.erase(m_items.end() - 1, m_items.end());
			return toReturn;
		}
		
	}
	return nullptr;
}
void Tile::AddItem(Interactable* _newItem)
{
	m_items.push_back(_newItem);
}
const Vector2& Tile::GetPositionInGrid() { return m_posInGrid; }



void Tile::InvokeMouseUp(MouseButton _mouse, Sint32 _x, Sint32 _y) 
{
	
	switch (_mouse)
	{
	case IInputHandler::MouseButton::LEFT:
		if (inBounds(_x, _y))
		{
			printf("clicked on tile %s (index %d), setting passable to %s\n", m_name.c_str(), GetPositionInVector(), (!IsPassable() ? "true." : "false."));
			SetPassable(!IsPassable());
		}
		break;
	case IInputHandler::MouseButton::RIGHT:
		if (inBounds(_x, _y))
		{
			printf("clicked on tile %s (index %d), \n", m_name.c_str(), GetPositionInVector());
			printf("Passable = %s\n", (IsPassable() ? "true." : "false."));
			printf("Corridor = %s\n", (IsCorridor() ? "true." : "false."));
			printf("Contains %s, \n", (m_contents != nullptr ? m_contents->GetName().c_str() : "Nothing"));
			if (m_items.size() > 0)
			{
				printf("Other items present include, ");
				for (size_t i = 0; i < m_items.size(); i++)
					printf("%s, ", m_items[i]->GetName().c_str());
				printf("\n");
			}

		}		
		break;
	case IInputHandler::MouseButton::MIDDLE:
		break;
	default:
		break;
	}
}

void Tile::ClearTileContents() {

	if (m_contents != nullptr)
	{
		if (m_contents->ShouldDelete())
			delete(m_contents);
		else
			m_contents->SetLocation(nullptr);
	}
	SetContents(nullptr);
	SetPassable(false);
	int size = m_items.size();
	for (size_t i = 0; i < size; i++)
	{
		if (m_items[i] != nullptr)
		{
			delete(m_items[i]);
			m_items[i] = nullptr;
		}
	}
	m_items.erase(m_items.begin(), m_items.end());
}

void Tile::Render(SDL_Renderer* _renderer)
{
	Renderable::Render(_renderer);
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i] != nullptr)
		{
			m_items[i]->SetPosition(GetPosition().X, GetPosition().Y);
			m_items[i]->Render(_renderer);
		}

	}
	if (m_contents != nullptr)
	{
		m_contents->SetPosition(GetPosition().X, GetPosition().Y);
		m_contents->Render(_renderer);
	}
	
	
}
