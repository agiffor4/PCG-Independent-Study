#include "Interactable.h"
#include "Tile.h"
bool Interactable::InteratctionWrapper(Thing* _thingInitatingInteraction)
{
	if (m_allowInteraction)
	{
		return Interaction(_thingInitatingInteraction);
	}
	return false;
}
void Interactable::Render(SDL_Renderer* _renderer, Uint8 _shade)
{
	storeTextureColorMod();
	setTextureColorMod({ _shade , _shade , _shade , 255 });
	Renderable::Render(_renderer);
	revertTextureColorMod();

}
Interactable::Interactable()
{
	SetRenderLayer(1);
}
Interactable::~Interactable()
{
}

void Interactable::SetBlocksPassage(bool _val)
{
	m_blocksPassage = _val;
}

bool Interactable::GetBlocksPassage()
{
	return m_blocksPassage;
}

bool Interactable::IsflaggedForDeletion()
{
	return m_flagForDelation;
}

bool Interactable::GetAllowInteraction()
{
	return m_allowInteraction;
}

void Interactable::InteractionOnEnteringTile(Thing* _thingInitatingInteraction)
{
}

void Interactable::Drop()
{
	m_shouldRender = true;
}

void Interactable::PickUp()
{
	m_shouldRender = false;
	if (GetLocation() != nullptr)
	{
		GetLocation()->RemoveItem(this, false);
		SetLocation(nullptr);
	}
}
