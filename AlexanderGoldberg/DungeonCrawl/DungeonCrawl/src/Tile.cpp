#include "Tile.h"
#include "Thing.h"
#include "World.h"
#include "Interactable.h"
#include "TextA.h"
#include "Camera.h"
#include "World.h"
#include "Shadow.h"

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
	SetPosition(GetDefaultSize().X * _startPos.X, GetDefaultSize().Y * _startPos.Y);
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
	if (_x > GetDestination().x - Camera::Offset().X && _x < GetDestination().x + GetDestination().w  - Camera::Offset().X &&
		_y > GetDestination().y - Camera::Offset().Y && _y < GetDestination().y + GetDestination().h - Camera::Offset().Y)
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
		int fromBack = 1;
		Interactable* toReturn = m_items[m_items.size() - fromBack];
		while (!toReturn->GetAllowInteraction())
		{
			fromBack++;
			int target = m_items.size() - fromBack;
			if (target > -1)
			{
				toReturn = m_items[target];
			}
			else
			{
				return nullptr;
			}
			
		}
		
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
	_newItem->SetScale(GetScale());
	_newItem->SetLocation(this);
	m_items.push_back(_newItem);
}

void Tile::AddShadow(Shadow* _newShadow)
{
	_newShadow->SetScale(GetScale());
	m_shadows.push_back(_newShadow);
	_newShadow->SetLocation(this);
}



Interactable* Tile::RemoveItem(Interactable* _toRemove, bool _deleteOnRemoval)
{
	Interactable* toReturn = nullptr;
	auto itt = m_items.begin();
	while (itt != m_items.end())
	{
		if ((*itt) == _toRemove)
		{
			m_items.erase(itt);
			if (_deleteOnRemoval)
				delete(_toRemove);
			else
				toReturn = _toRemove;
			break;
		}
		itt++;
	}
	return toReturn;
}
const Vector2& Tile::GetPositionInGrid() { return m_posInGrid; }

void Tile::AddRoomNumber(int _roomTileIsIn, bool _renderNumbers, SDL_Renderer* _renderer)
{
	m_roomIn = _roomTileIsIn;
	if (_renderNumbers) 
	{
		if (m_text == nullptr)
		{
			m_text = new TextA();
			m_text->InitializeFont("fonts/FreeSans.ttf", 12, _renderer, { 0,0,0, 255 });
			m_text->SetPosition(GetPosition() + Vector2(2, 0));
		}
		m_text->SetText(std::to_string(_roomTileIsIn));
	}
}



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
			PrintTileData();
		}
		break;
	case IInputHandler::MouseButton::MIDDLE:
		break;
	default:
		break;
	}
}

void Tile::PrintTileData()
{
	auto lam = [](TileRenderType _type)
	{
		switch (_type)
		{
		case Tile::TileRenderType::empty:
			return "empty";
		case Tile::TileRenderType::wall4Side:
			return "wall";
		case Tile::TileRenderType::wall3Sidetop:
			return "TC3";
		case Tile::TileRenderType::wall3SideBottom:
			return "BC3";
		case Tile::TileRenderType::wall2SideTopL:
			return "TLC";
		case Tile::TileRenderType::wall2SideTopR:
			return "TRC";
		case Tile::TileRenderType::wall2SideBottomL:
			return "BLC";
		case Tile::TileRenderType::wall2SideBottomR:
			return "BRC";
		case Tile::TileRenderType::wall3SideLeft:
			return "LC3";
		case Tile::TileRenderType::wall3SideRight:
			return "RC3";	
		case Tile::TileRenderType::wall1SideBottom:
			return "W1B";
		case Tile::TileRenderType::wall1SideTop:
			return "W1T";
		case Tile::TileRenderType::wall1SideLeft:
			return "W1L";
		case Tile::TileRenderType::wall1SideRight:
			return "W1R";
		case Tile::TileRenderType::wall2SideUpDown:
			return "W2UD";
		case Tile::TileRenderType::wall2SideLeftRight:
			return "w2LR";
		default:
			return "Error!";
		}
	};
	printf("\nClicked on tile %s (index %d) in room with index %d, \n", m_name.c_str(), GetPositionInVector(), m_roomIn);
	printf("Passable = %s\n", (IsPassable() ? "true." : "false."));
	printf("Tile render type is %s\n", lam(m_tilerRenderType));
	printf("Corridor = %s\n", (IsCorridor() ? "true." : "false."));
	printf("Contains %s, \n", (m_contents != nullptr ? m_contents->GetName().c_str() : "Nothing"));
	if (m_items.size() > 0)
	{
		printf("Other items present include, ");
		for (size_t i = 0; i < m_items.size(); i++)
			printf("%s, ", m_items[i]->GetName().c_str());
		printf("\n");
	}
	if (m_shadows.size() > 0)
	{
		printf("Shadows present include, ");
		for (size_t i = 0; i < m_shadows.size(); i++)
			printf("%s, ", m_shadows[i]->GetName().c_str());
		printf("\n");
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
	if(m_text != nullptr)
		m_text->CleanUp();

	size = m_shadows.size();
	for (size_t i = 0; i < size; i++)
	{
		if (m_shadows[i] != nullptr)
		{
			delete(m_shadows[i]);
			m_shadows[i] = nullptr;
		}
	}
	m_shadows.erase(m_shadows.begin(), m_shadows.end());
	m_illuminated = false;
	m_inFogOfWar = true;
	m_roomIn = -1;
}

void Tile::Render(SDL_Renderer* _renderer)
{
	Renderable::Render(_renderer);
	if (!m_renderFogOfWar || !m_inFogOfWar)
	{
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
	


	if (!m_illuminated)
	{
		for (size_t i = 0; i < (m_inFogOfWar && m_renderFogOfWar ? m_shadows.size() : m_shadows.size() -1); i++)
		{
			if (m_shadows[i] != nullptr)
			{
				m_shadows[i]->SetPosition(GetPosition().X, GetPosition().Y);
				m_shadows[i]->Render(_renderer);
			}
		}
	}
	

	if (m_text != nullptr)
	{
		m_text->Render(_renderer);

	}
	
}

void Tile::Update(float _dt)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i] != nullptr)
		{			
			m_items[i]->Update(_dt);
		}

	}
	if (m_contents != nullptr)
	{		
		m_contents->Update(_dt);
	}

	for (int i = m_items.size() -1; i > -1; i--)
	{
		if (m_items[i] != nullptr && m_items[i]->IsflaggedForDeletion())
		{			
			RemoveItem(m_items[i], true);
		}

	}
}

void Tile::DetermineTileType(World* _world)
{

	m_tilerRenderType = TileRenderType::empty;
	Uint8 flag = 0;
	Tile* t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::UP);
	if(t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::above;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::DOWN);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::below;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::LEFT);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::left;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::RIGHT);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::right;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::UPRIGHT);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::northeast;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::UPLEFT);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::northwest;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::DOWNRIGHT);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::southeast;
	t = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::DOWNLEFT);
	if (t == nullptr || !t->IsPassable(true))
		flag |= (Uint8)WallNeigbors::southwest;

	if (flag & (Uint8)WallNeigbors::below && (flag ^ (Uint8)WallNeigbors::above) && (flag ^ (Uint8)WallNeigbors::right) && (flag ^ (Uint8)WallNeigbors::left))
		m_tilerRenderType = TileRenderType::wall1SideBottom;
	if (flag & (Uint8)WallNeigbors::above && (flag ^ (Uint8)WallNeigbors::below) && (flag ^ (Uint8)WallNeigbors::right) && (flag ^ (Uint8)WallNeigbors::left))
		m_tilerRenderType = TileRenderType::wall1SideTop;
	if (flag & (Uint8)WallNeigbors::right && (flag ^ (Uint8)WallNeigbors::below) && (flag ^ (Uint8)WallNeigbors::left) && (flag ^ (Uint8)WallNeigbors::above))
		m_tilerRenderType = TileRenderType::wall1SideRight;
	if (flag & (Uint8)WallNeigbors::left && (flag ^ (Uint8)WallNeigbors::below) && (flag ^ (Uint8)WallNeigbors::right) && (flag ^ (Uint8)WallNeigbors::above))
		m_tilerRenderType = TileRenderType::wall1SideLeft;
	if (flag & (Uint8)WallNeigbors::left && flag & (Uint8)WallNeigbors::below)
		m_tilerRenderType = TileRenderType::wall2SideBottomL;
	if (flag & (Uint8)WallNeigbors::right && flag & (Uint8)WallNeigbors::below)
		m_tilerRenderType = TileRenderType::wall2SideBottomR;
	if (flag & (Uint8)WallNeigbors::left && flag & (Uint8)WallNeigbors::above)
		m_tilerRenderType = TileRenderType::wall2SideTopL;
	if (flag & (Uint8)WallNeigbors::right && flag & (Uint8)WallNeigbors::above)
		m_tilerRenderType = TileRenderType::wall2SideTopR;
	if (flag & (Uint8)WallNeigbors::right && flag & (Uint8)WallNeigbors::left && flag & (Uint8)WallNeigbors::below)
		m_tilerRenderType = TileRenderType::wall3SideBottom;
	if (flag & (Uint8)WallNeigbors::right && flag & (Uint8)WallNeigbors::left && flag & (Uint8)WallNeigbors::above)
		m_tilerRenderType = TileRenderType::wall3Sidetop;
	if (flag & (Uint8)WallNeigbors::left && flag & (Uint8)WallNeigbors::below && flag & (Uint8)WallNeigbors::right && flag & (Uint8)WallNeigbors::above)
		m_tilerRenderType = TileRenderType::wall4Side;	
	if (flag & (Uint8)WallNeigbors::left && flag & (Uint8)WallNeigbors::below && flag & (Uint8)WallNeigbors::above)
		m_tilerRenderType = TileRenderType::wall3SideLeft;
	if (flag & (Uint8)WallNeigbors::below && flag & (Uint8)WallNeigbors::right && flag & (Uint8)WallNeigbors::above)
		m_tilerRenderType = TileRenderType::wall3SideRight;	
	if (flag & (Uint8)WallNeigbors::left && (flag & (Uint8)WallNeigbors::right) && (flag ^ (Uint8)WallNeigbors::below) && (flag ^ (Uint8)WallNeigbors::above))
		m_tilerRenderType = TileRenderType::wall2SideLeftRight;
	if (flag ^ (Uint8)WallNeigbors::left && (flag ^ (Uint8)WallNeigbors::right) && (flag & (Uint8)WallNeigbors::below) && (flag & (Uint8)WallNeigbors::above))
		m_tilerRenderType = TileRenderType::wall2SideUpDown;

		Tile* adj = _world->GetAdjacentTile(GetPositionInVector(), World::TileDirection::UP);
		if (adj != nullptr)
		{
			if (IsPassable(true) == adj->IsPassable(true))// if tile above has the same passailty as this one
			{
				
				SetPosition(Vector2(adj->GetPosition().X, adj->GetPosition().Y + GetDestination().w));
			}
			else
			{ 
				if (IsPassable(true))// if not a wall
				{
					SetPosition(Vector2(adj->GetPosition().X, adj->GetPosition().Y + GetDestination().w + 12));
				}
				else
				{
					SetPosition(Vector2(adj->GetPosition().X, adj->GetPosition().Y + GetDestination().w - 12));
				}
			
			}
		}
		
		if (IsPassable(true))
		{
			Shadow* s = nullptr;
			switch (m_tilerRenderType)
			{
			case Tile::TileRenderType::empty:
				break;
			case Tile::TileRenderType::wall4Side:
				break;
			case Tile::TileRenderType::wall3Sidetop:
				break;
			case Tile::TileRenderType::wall3SideBottom:
				break;
			case Tile::TileRenderType::wall2SideTopL:
				s = new Shadow();
				s->Init("img/Shadow_West.png", "Shadow North West 1", m_rendererRef);
				AddShadow(s);
				s = new Shadow();
				s->Init("img/Shadow_South.png", "Shadow North West 2", m_rendererRef);
				break;
			case Tile::TileRenderType::wall2SideTopR:
				s = new Shadow();
				s->Init("img/Shadow_East.png", "Shadow North East 1", m_rendererRef);
				AddShadow(s);
				s = new Shadow();
				s->Init("img/Shadow_South.png", "Shadow North East 2", m_rendererRef);
				break;
			case Tile::TileRenderType::wall2SideBottomL:
				s = new Shadow();
				s->Init("img/Shadow_West.png", "Shadow South West 1", m_rendererRef);
				AddShadow(s);
				s = new Shadow();
				s->Init("img/Shadow_North.png", "Shadow South West 2", m_rendererRef);
				break;
			case Tile::TileRenderType::wall2SideBottomR:
				s = new Shadow();
				s->Init("img/Shadow_East.png", "Shadow South East 1", m_rendererRef);
				AddShadow(s);
				s = new Shadow();
				s->Init("img/Shadow_North.png", "Shadow South East 2", m_rendererRef);
				break;
			case Tile::TileRenderType::wall3SideLeft:
				break;
			case Tile::TileRenderType::wall3SideRight:
				break;
			case Tile::TileRenderType::wall1SideBottom:
				s = new Shadow();
				s->Init("img/Shadow_North.png", "Shadow North", m_rendererRef);
				break;
			case Tile::TileRenderType::wall1SideTop:
			case Tile::TileRenderType::wall2SideUpDown:
				s = new Shadow(); 
				s->Init("img/Shadow_South.png", "Shadow South", m_rendererRef);				
				break;
			case Tile::TileRenderType::wall1SideLeft:
				s = new Shadow();
				s->Init("img/Shadow_West.png", "Shadow West", m_rendererRef);
				break;
			case Tile::TileRenderType::wall1SideRight:
			case Tile::TileRenderType::wall2SideLeftRight:
				s = new Shadow();
				s->Init("img/Shadow_East.png", "Shadow East", m_rendererRef);
				break;
			default:
				break;
			}
			if (s != nullptr)
				AddShadow(s);

			if (flag & (Uint8)WallNeigbors::northeast 
				&& !(flag & (Uint8)WallNeigbors::right)
				&& !(flag & (Uint8)WallNeigbors::above)
				&& !(flag & (Uint8)WallNeigbors::northwest)
			   )
			{
				s = new Shadow();
				s->Init("img/Shadow_North_East.png", "Shadow North East", m_rendererRef);
				AddShadow(s);
			}

			if (flag & (Uint8)WallNeigbors::southeast 
				&& !(flag & (Uint8)WallNeigbors::below)
				&& !(flag & (Uint8)WallNeigbors::right))
			{
				s = new Shadow();
				s->Init("img/Shadow_South_East.png", "Shadow South East", m_rendererRef);
				AddShadow(s);
			}
			if (flag & (Uint8)WallNeigbors::northwest 
				&& !(flag & (Uint8)WallNeigbors::left)
				&& !(flag & (Uint8)WallNeigbors::above)
				&& !(flag & (Uint8)WallNeigbors::northeast)
				) 
			{
				s = new Shadow();
				s->Init("img/Shadow_North_West.png", "Shadow North West", m_rendererRef);
				AddShadow(s);
			}
			
			if (flag & (Uint8)WallNeigbors::southwest 
				&& !(flag & (Uint8)WallNeigbors::below)
				&& !(flag & (Uint8)WallNeigbors::left)
				&& !(flag & (Uint8)WallNeigbors::southeast)
				)
			{
				s = new Shadow();
				s->Init("img/Shadow_South_West.png", "Shadow South West", m_rendererRef);
				AddShadow(s);
				
			}
		}
		Shadow* s = new Shadow();
		s->Init("img/FogOfWar.png", "Fog Of War", m_rendererRef);
		AddShadow(s);
}

void Tile::SetIlluminated(bool _illuminated)
{
	m_illuminated = _illuminated;
}

void Tile::SetFogOfWar(bool _inFogOfWar)
{ 
	if (!m_illuminated)
	{
		m_inFogOfWar = _inFogOfWar;
	}
	else
	{
		m_inFogOfWar = false;
	}
}
