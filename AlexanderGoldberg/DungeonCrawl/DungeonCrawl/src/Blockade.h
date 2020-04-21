#pragma once
#include "Interactable.h"
class Timer;
class Scene;
class Blockade :
	public Interactable
{
protected:
	Timer* m_lifetime = nullptr;
	void die();
public:
	Blockade();
	~Blockade();
	void CleanUp();
	void SetLifeTime(float _min, float _max);
	void Initialize();
	void Update(float _dt) override;
	bool Interaction(Thing* _thingInitatingInteraction) override;
};

