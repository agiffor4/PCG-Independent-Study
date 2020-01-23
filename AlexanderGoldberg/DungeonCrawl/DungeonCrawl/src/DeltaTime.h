#pragma once
#include <chrono>
#define a_clock std::chrono::high_resolution_clock
class DeltaTime
{
private:
	//setting up deltaTime
	
	std::chrono::time_point<std::chrono::steady_clock> m_currentTime = a_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> m_newTime = a_clock::now();
public:
	DeltaTime();
	~DeltaTime();
	float GetCurrentDeltaTime();
};

