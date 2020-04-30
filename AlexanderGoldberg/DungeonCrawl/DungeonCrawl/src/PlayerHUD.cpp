#include "PlayerHUD.h"
#include "Player.h"
#include "Scene.h"
#include "TextA.h"
#include "Healthbar.h"
PlayerHUD::PlayerHUD() {}

PlayerHUD::~PlayerHUD()
{
}

void PlayerHUD::Initialize(Player* _player, SDL_Renderer* _renderer, Scene* _scene)
{

	m_player = _player;
	m_healthBar = new Healthbar();
	m_ammoCount = new TextA();
	m_levelCount = new TextA();
	m_healthBar->Init("img/HealthBar.png", "Healthbar", _renderer);
	m_healthBar->SetPosition(0, 0);
	m_healthBar->SetHealthPercent(m_player->GetHealthAsPercent());
	m_ammoCount->InitializeFont("fonts/FreeSans.ttf", 12, _renderer, { 255, 0, 0,255 });
	m_ammoCount->SetPosition(0, 32);
	m_ammoCount->SetShouldRender(true);
	m_levelCount->InitializeFont("fonts/FreeSans.ttf", 24, _renderer, { 255, 0, 0,255 });
	m_levelCount->SetPosition(608, 0);
	m_levelCount->SetShouldRender(true);
	_scene->AddUI(m_levelCount);
	_scene->AddUI(m_ammoCount);
	_scene->AddUI(m_healthBar);
}
void PlayerHUD::Update(float _dt)
{
	m_healthBar->SetHealthPercent(m_player->GetHealthAsPercent());
	std::string result = m_player->GetAmmo() > 0 ? std::to_string(m_player->GetAmmo()) : "empty";
	m_ammoCount->SetText(result);
	m_levelCount->SetText("Level: " + std::to_string(m_player->GetLevelGenCount()));
	m_levelCount->SetPosition(640 - m_levelCount->getTextWidth(), 0);
}

