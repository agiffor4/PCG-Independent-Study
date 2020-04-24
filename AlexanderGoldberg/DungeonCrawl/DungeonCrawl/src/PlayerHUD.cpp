#include "PlayerHUD.h"
#include "Player.h"
PlayerHUD::PlayerHUD(Player* _player, SDL_Renderer* _renderer)
{
	m_player = _player;
	m_healthBar.Init("img/HealthBar.png", "Healthbar", _renderer);
	m_healthBar.SetPosition(0, 0);
	m_healthBar.SetHealthPercent(m_player->GetHealthAsPercent());
	m_ammoCount.InitializeFont("fonts/FreeSans.ttf", 12, _renderer, {255, 0, 0,255});
	m_ammoCount.SetPosition(0, 32);
}

PlayerHUD::~PlayerHUD()
{
}

void PlayerHUD::Update(float _dt)
{
	m_healthBar.SetHealthPercent(m_player->GetHealthAsPercent());
	std::string result = m_player->GetAmmo() > 0 ? std::to_string(m_player->GetAmmo()) : "empty";
	m_ammoCount.SetText(result);
}

void PlayerHUD::Render(SDL_Renderer* _renderer)
{
	m_healthBar.Render(_renderer);
	m_ammoCount.Render(_renderer);
}
