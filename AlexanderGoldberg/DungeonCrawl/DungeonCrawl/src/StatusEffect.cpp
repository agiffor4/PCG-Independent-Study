#include "StatusEffect.h"
#include "StatusEffectable.h"
bool StatusEffect::effectInternal(StatusEffectable* _toEffect)
{
	return true;
}
StatusEffect::StatusEffect()
{
}

StatusEffect::~StatusEffect()
{
}

void StatusEffect::InitalizeStatus(float _lifeTimeSeconds, float _incrementForEffectToOccurSeconds)
{
	m_lifeTimeSecs.SetTimer(_lifeTimeSeconds);
	m_effectIncrementsSecs.SetTimer(_incrementForEffectToOccurSeconds);
}


bool StatusEffect::Effect(StatusEffectable* _toEffect, float _dt)
{
	if (m_lifeTimeSecs.CountDown(_dt))
	{
		return true;
	}
	if (m_effectIncrementsSecs.CountDown(_dt))
	{
		return effectInternal(_toEffect);
	}
	return false;
}

float StatusEffect::GetEffectFrequency()
{
	return m_effectIncrementsSecs.GetResetTime();
}

float StatusEffect::GetEffectLifeTime()
{
	return m_lifeTimeSecs.GetResetTime();
}
