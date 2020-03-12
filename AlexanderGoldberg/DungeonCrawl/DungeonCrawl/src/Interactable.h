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
	virtual bool Interaction() = 0; //returns false if it should not be added to the player inventory ineteraction
	void SetBlocksPassage(bool _val);
	bool GetBlocksPassage();
	bool IsflaggedForDeletion();
	bool InteratctionWrapper();
};

