#pragma once
#include "TextA.h"
#include "Healthbar.h"
class Player;
class PlayerHUD
{
protected:
	Player* m_player = nullptr;
	TextA m_ammoCount = TextA();
	Healthbar m_healthBar = Healthbar();
public:

	PlayerHUD(Player* _player, SDL_Renderer* _renderer);
	~PlayerHUD();
	void Update(float _dt);
	void Render(SDL_Renderer* _renderer);
	

};

