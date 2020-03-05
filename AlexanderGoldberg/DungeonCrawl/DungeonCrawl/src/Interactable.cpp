#include "Interactable.h"

bool Interactable::InteratctionWrapper()
{
	if (m_allowInteraction)
	{
		return Interaction();
	}
	return false;
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
