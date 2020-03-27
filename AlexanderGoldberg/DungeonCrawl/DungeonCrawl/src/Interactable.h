#pragma once
#include "Thing.h"
class Interactable :
	public Thing
{
protected:
	bool m_allowInteraction = true;
	bool m_blocksPassage = false;
	bool m_flagForDelation = false;
public:
	Interactable();
	virtual ~Interactable();
	virtual bool Interaction(Thing* _thingInitatingInteraction) = 0; //returns false if it should not be added to the player inventory ineteraction
	void SetBlocksPassage(bool _val);
	bool GetBlocksPassage();
	bool IsflaggedForDeletion();
	bool GetAllowInteraction();
	virtual void InteractionOnEnteringTile(Thing* _thingInitatingInteraction);
	bool InteratctionWrapper(Thing* _thingInitatingInteraction);
	void Render(SDL_Renderer* _renderer, Uint8 _shade);
};


