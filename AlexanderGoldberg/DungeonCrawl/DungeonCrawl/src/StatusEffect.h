#pragma once
#include "Timer.h"
class StatusEffectable;
class StatusEffect
{
protected:
	Timer m_lifeTimeSecs = Timer(1.0f);
	Timer m_effectIncrementsSecs = Timer(1.0f);
	virtual bool effectInternal(StatusEffectable* _toEffect);
public:
	StatusEffect();
	virtual ~StatusEffect();
	virtual void InitalizeStatus(float _lifeTimeSeconds, float _incrementForEffectToOccurSeconds);
	bool Effect(StatusEffectable* _toEffect, float _dt);
	float GetEffectFrequency();
	float GetEffectLifeTime();

};

