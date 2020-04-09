#include "Timer.h"
#include<iostream>


Timer::Timer(float startTime)
{
	m_resetTime = startTime;
	m_time = startTime;
}


Timer::~Timer()
{
}

bool Timer::CountDown(float dt) {
	m_time -= dt;
	if (m_time <= 0)
	{
		m_time = m_resetTime;
		return true;

	}
	return false;
}
bool Timer::CountDownAutoCheckBool(float dt) {
	if (m_shouldCountDown)
	{
		bool tempVal = CountDown(dt);
		if (tempVal)
		{
			m_shouldCountDown = false;			
		}
		return tempVal;
	}
	return false;
}
bool Timer::GetShouldCountDown() { return m_shouldCountDown; }
void Timer::SetShouldCountDown(bool val) { m_shouldCountDown = val; }
void Timer::SetTimer(float time) {
	m_resetTime = time;
	m_time = time;
}
void Timer::SetCurrentTime(float time) {
	m_time = time;
}

float Timer::GetResetTime()
{
	return m_resetTime;
}

float Timer::GetCurrentTime()
{
	return m_time;
}
