#pragma once
#include "Thing.h"
class Interactable :
	public Thing
{
protected:
	bool m_allowInteraction = true;
	bool m_blocksPassage = false;
public:
	virtual bool Interaction() = 0;
	void SetBlocksPassage(bool _val);
	bool GetBlocksPassage();
	bool InteratctionWrapper();
};

