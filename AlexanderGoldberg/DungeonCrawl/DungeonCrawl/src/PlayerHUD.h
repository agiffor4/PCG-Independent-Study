#pragma once
class TextA;
class Healthbar;
class Player;
class Scene;
struct SDL_Renderer;
class PlayerHUD
{
protected:
	Player* m_player = nullptr;
	TextA* m_ammoCount = nullptr;
	TextA* m_levelCount = nullptr;
	Healthbar* m_healthBar = nullptr;
public:
	void Initialize(Player* _player, SDL_Renderer* _renderer, Scene* _scene);
	PlayerHUD();
	~PlayerHUD();
	void Update(float _dt);
	

};

