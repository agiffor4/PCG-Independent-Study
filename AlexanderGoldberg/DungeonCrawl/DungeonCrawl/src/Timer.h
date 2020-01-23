#pragma once
class Timer
{
private:
	float m_time;
	float m_resetTime;
	bool m_shouldCountDown;
protected:
public:
	bool CountDown(float dt);
	bool CountDownAutoCheckBool(float dt);
	bool GetShouldCountDown();
	void SetShouldCountDown(bool val);
	void SetTimer(float time);
	void SetCurrentTime(float time);
	Timer(float startTime);
	~Timer();
};

