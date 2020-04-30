#include "WeaponNameRenderer.h"
#include "Vector2.h"
bool WeaponNameRenderer::UpdateB(float _dt) 
{
	SetPosition(GetPosition() + (Vector2(0, -100) * _dt));
	if (GetPosition().Y < 0)
		return true;
	return false;
}